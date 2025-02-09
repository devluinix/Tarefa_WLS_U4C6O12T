# Controle de Matriz de LEDs 5x5 WS2812 com Interação via Serial e Botões

## Descrição do Projeto

Este projeto utiliza a placa BitDogLab com o microcontrolador RP2040 para controlar uma matriz 5x5 de LEDs WS2812, um LED RGB e um display SSD1306. A interação é realizada via monitor serial e dois botões físicos, proporcionando funcionalidades de exibição de caracteres, alternância de LEDs e registro de eventos no display e no monitor serial.

### Componentes Utilizados:

1. **Matriz 5x5 de LEDs WS2812** - Conectada à GPIO 7.
2. **LED RGB** - Conectado às GPIOs 11, 12 e 13.
3. **Botão A** - Conectado à GPIO 5.
4. **Botão B** - Conectado à GPIO 6.
5. **Display OLED SSD1306 (128x64)** - Conectado via I2C (GPIO 14 e GPIO 15).

## DEMONSTRAÇÃO

[Vídeo de Demonstração](https://youtube.com/shorts/9kd9Mg821c4)

## Funcionalidades

### 1. Modificação da Biblioteca `font.h`

- Adicionar caracteres minúscusulos personalizados à biblioteca `font.h` para exibição no display SSD1306.

### 2. Entrada de Caracteres via Monitor Serial

- O Serial Monitor do VS Code é utilizado para enviar caracteres.
- O caractere digitado é exibido no display SSD1306.
- Se um número de 0 a 9 for digitado, um símbolo correspondente ao número é exibido na matriz 5x5 WS2812.

### 3. Interação com o Botão A

- Alterna o estado do LED RGB Verde (ligado/desligado).
- Exibe uma mensagem informativa no display SSD1306.
- Registra a operação no Serial Monitor.

### 4. Interação com o Botão B

- Alterna o estado do LED RGB Azul (ligado/desligado).
- Exibe uma mensagem informativa no display SSD1306.
- Registra a operação no Serial Monitor.

## Requisitos do Projeto

1. **Uso de Interrupções (IRQ)** - Todas as funcionalidades dos botões devem ser implementadas utilizando rotinas de interrupção.
2. **Debouncing** - É obrigatório o tratamento do bouncing dos botões via software.
3. **Controle de LEDs** - Implementação do controle de LEDs comuns e LEDs WS2812.
4. **Utilização do Display SSD1306** - Exibição de caracteres maiúscusulos e minúscusulos, demonstrando o entendimento do protocolo I2C.
5. **Envio de Informações via UART** - Comunicação serial entre o microcontrolador e o PC.
6. **Organização do Código** - O código deve ser bem estruturado e comentado.

## Como Compilar

Para compilar o programa, utilize um compilador C compatível com o Raspberry Pi Pico W e gere os arquivos `.uf2` e `.elf`:

1. Configure o ambiente de desenvolvimento.
2. Compile o código utilizando um compilador compatível (ex: `arm-none-eabi-gcc`).

## Como Executar

1. Carregue o arquivo `.uf2` no Raspberry Pi Pico W.
2. Utilize o Wokwi para simular o circuito abrindo o arquivo `diagram.json` e clicando em "Play".
3. Utilize o Serial Monitor do VS Code para testar a entrada de caracteres.

## Desenvolvedores

**Equipe 2 - Grupo 3 - EmbarcaTech**

- [Luiz Marcelo](https://github.com/devluinix)
