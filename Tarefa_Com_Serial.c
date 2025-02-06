#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812.pio.h"

// ---------------- Variáveis - Início ----------------



// ---------------- Variáveis - Fim ----------------



// ---------------- Defines - Início ----------------

#define UART_ID uart0

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define green_button 5 // Define o pino do botão verde.
#define red_button 6   // Define o pino do botão vermelho.

#define red_rgb 13     // Define o pino vermelho do LED.
#define green_rgb 11   // Define o pino verde do LED.
#define blue_rgb 12    // Define o pino azul do LED.

#define luz 1        // Define a intensidade da luz (1-255)

// ---------------- Defines - Fim ----------------



// ---------------- WS2812 - Início ----------------

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

// Definição de pixel GRB
struct pixel_t
{
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

// Inicializa a máquina PIO para controle da matriz de LEDs.
void npInit(uint pin)
{

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2812_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0)
  {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2812_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

// Atribui uma cor RGB a um LED.
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

// Limpa o buffer de pixels.
void npClear()
{
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

// Escreve os dados do buffer nos LEDs.
void npWrite()
{
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

// ---------------- WS2812 - Fim ----------------



// ---------------- Desenhar - Início ----------------

// Função para facilitar o desenho no WS2812 utilizando 3 matrizes para o R, G e B.
void npDraw(uint8_t vetorR[5][5], uint8_t vetorG[5][5], uint8_t vetorB[5][5])
{
  int i, j,idx,col;
    for (i = 0; i < 5; i++) {
        idx = (4 - i) * 5; // Calcula o índice base para a linha.
        for (j = 0; j < 5; j++) {
            col = (i % 2 == 0) ? (4 - j) : j; // Inverte a ordem das colunas nas linhas pares.
            npSetLED(idx + col, vetorR[i][j], vetorG[i][j], vetorB[i][j]); // Preenche o buffer com os valores da matriz.
        }
    }
}

// ---------------- Desenhar - Fim ----------------



// ---------------- Números - Início ----------------

uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
};

void num_0() {
  uint8_t vetorB[5][5] = {
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorRG,vetorRG,vetorB); // Carrega os buffers.
  npWrite();                      // Escreve na matriz de LEDS.
  npClear();                      // Limpa os buffers (não necessário, mas por garantia).
}

void num_1() {
  uint8_t vetorB[5][5] = {
    {  0  ,  0  , luz ,  0  ,  0  },
    {  0  , luz , luz ,  0  ,  0  },
    {  0  ,  0  , luz ,  0  ,  0  },
    {  0  ,  0  , luz ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_2() {
  uint8_t vetorB[5][5] = {
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_3() {
  uint8_t vetorB[5][5] = {
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_4() {
  uint8_t vetorB[5][5] = {
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_5() {
  uint8_t vetorB[5][5] = {
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_6() {
  uint8_t vetorB[5][5] = {
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_7() {
  uint8_t vetorB[5][5] = {
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_8() {
  uint8_t vetorB[5][5] = {
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_9() {
  uint8_t vetorB[5][5] = {
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

// Função para para exibir o número passado como parâmetro na matriz de LEDS.
int handle_numbers(char num) {
  switch(num) {
    case '0':
      num_0();
      break;
    case '1':
      num_1();
      break;
    case '2':
      num_2();
      break;
    case '3':
      num_3();
      break;
    case '4':
      num_4();
      break;
    case '5':
      num_5();
      break;
    case '6':
      num_6();
      break;
    case '7':
      num_7();
      break;
    case '8':
      num_8();
      break;
    case '9':
      num_9();
      break;
    default:
      npClear();
      npWrite();
      return 1;
  }
  printf("%c Escrito na matriz!\n", num); // Print para visualização no terminal.
  return 0;
}

// ---------------- Números - Fim ----------------



// ---------------- Inicializações - Início ----------------

void start_display(ssd1306_t *ssd) {
    
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line

    ssd1306_init(ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(ssd); // Configura o display
    ssd1306_send_data(ssd); // Envia os dados para o display
    
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
}

// Inicializa os botões configurando os pinos apropriados.
void init_buttons() {
  gpio_init(red_button);
  gpio_init(green_button);
  gpio_set_dir(red_button, GPIO_IN);
  gpio_set_dir(green_button, GPIO_IN);
  gpio_pull_up(red_button);
  gpio_pull_up(green_button);
}

// Inicializa o LED RGB configurando os pinos apropriados.
void init_RGB() {
  gpio_init(red_rgb);
  gpio_init(green_rgb);
  gpio_init(blue_rgb);
  gpio_set_dir(red_rgb,GPIO_OUT);
  gpio_set_dir(green_rgb,GPIO_OUT);
  gpio_set_dir(blue_rgb,GPIO_OUT);
  gpio_put(red_rgb, 0);
  gpio_put(green_rgb, 0);
  gpio_put(blue_rgb, 0);
}

// ---------------- Inicializações  - Fim ----------------

int main() {
  ssd1306_t ssd; // Inicializa a estrutura do display

  stdio_init_all();

  uart_init(UART_ID, 115200);
  gpio_set_function(0, GPIO_FUNC_UART);
  gpio_set_function(1, GPIO_FUNC_UART);

  start_display(&ssd);

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  npClear();
  npWrite();

  // Inicializa os botões.
  init_buttons();

  // Inicializa o LED RGB.
  init_RGB();

  while(true) {
    sleep_ms(20);
  }
}
