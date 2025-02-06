# Tarefa do EmbarcaTech para introduzir comunicações seriais com o uso da BitDogLab
  - **Desenvolvedor:** <ins>Gabriel Cavalcanti Coelho</ins>;
  - **Vídeo:** [YouTube](https://youtu.be/cgg5ZuJm_Vc?si=QXdBF2tKewpJKKAv).

### Este projeto implementa o controle de um display OLED SSD1306 e de uma matriz de LEDs WS2812 através do terminal serial do VS Code. Além disso, também inclui interrupções por botões para acionar um LED RGB e debouncing por software.

**Funcionalidades:**
  - Controle de um display OLED via I2C;
  - Gerenciamento de uma matriz de LEDs WS2812 para exibição de números;
  - Interação por meio de botões físicos para alternância de LEDs RGB;
  - Comunicação via UART para recebimento de caracteres.

**Interação:**
  - Envie caracteres pelo terminal para exibi-los no display OLED e na matriz de LEDs;
  - Pressione o **botão A** para **alternar** o estado da **cor verde** do LED RGB;
  - Pressione o **botão B** para **alternar** o estado da **cor azul** do LED RGB.

**Observações:**
  - O superloop também contém o código adaptado para o simulador Wokwi, que está comentado (entre /* e */). Para utilizá-lo é recomendado comentar o código feito para a BitDogLab, tanto para evitar bugs como para melhor desempenho do simulador, que fica bem lento ao rodar os dois códigos juntos.
