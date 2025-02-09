#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "hardware/pio.h"
#include "matriz_leds.h"
#include "inc/ssd1306.h"
#include "pico/time.h"
#include "inc/font.h"
#include <locale.h>

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define BUTTON_A 5
#define BUTTON_B 6
#define DEBOUNCE_TIME 200
bool mudarMatriz = false;             // Flag para atualizar matriz no main
uint32_t tempo_botao_pressionado = 0; // Tempo de pressionamento do botão em ms

#define LED_BLUE 11
#define LED_GREEN 12

bool ledBlue = false;
bool ledGreen = false;

ssd1306_t ssd;
char charDigitado = 'A';

void actionMatriz(int key, PIO pio, uint sm)
{
  const RGB_cod red = {20, 0, 0};
  const RGB_cod blk = {0, 0, 0};
  if (key == ' ')
  {
    Matriz_leds_config frame = {
        {blk, blk, blk, blk, blk},
        {blk, blk, blk, blk, blk},
        {blk, blk, blk, blk, blk},
        {blk, blk, blk, blk, blk},
        {blk, blk, blk, blk, blk}};
    imprimir_desenho(frame, pio, sm);
  }
  else if (key == '1')
  {
    Matriz_leds_config frame = {
        {blk, blk, red, blk, blk},
        {blk, red, red, blk, blk},
        {blk, blk, red, blk, blk},
        {blk, blk, red, blk, blk},
        {blk, red, red, red, blk}};
    imprimir_desenho(frame, pio, sm);
  }
  else if (key == '2')
  {
    Matriz_leds_config frame = {
        {blk, red, red, red, blk},
        {blk, blk, blk, red, blk},
        {blk, red, red, blk, blk},
        {blk, red, blk, blk, blk},
        {blk, red, red, red, blk}};
    imprimir_desenho(frame, pio, sm);
  }
  else if (key == '3')
  {
    Matriz_leds_config frame = {
        {blk, red, red, red, blk},
        {blk, blk, blk, red, blk},
        {blk, blk, red, red, blk},
        {blk, blk, blk, red, blk},
        {blk, red, red, red, blk}};
    imprimir_desenho(frame, pio, sm);
  }
  else if (key == '4')
  {
    Matriz_leds_config frame = {
        {blk, red, blk, red, blk},
        {blk, red, blk, red, blk},
        {blk, red, red, red, blk},
        {blk, blk, blk, red, blk},
        {blk, blk, blk, red, blk}};
    imprimir_desenho(frame, pio, sm);
  }
  else if (key == '5')
  {
    Matriz_leds_config frame = {
        {blk, red, red, red, blk},
        {blk, red, blk, blk, blk},
        {blk, red, red, red, blk},
        {blk, blk, blk, red, blk},
        {blk, red, red, red, blk}};
    imprimir_desenho(frame, pio, sm);
  }
  else if (key == '6')
  {
    Matriz_leds_config frame = {
        {blk, red, red, red, blk},
        {blk, red, blk, blk, blk},
        {blk, red, red, red, blk},
        {blk, red, blk, red, blk},
        {blk, red, red, red, blk}};
    imprimir_desenho(frame, pio, sm);
  }
  else if (key == '7')
  {
    Matriz_leds_config frame = {
        {blk, red, red, red, blk},
        {blk, blk, blk, red, blk},
        {blk, blk, blk, red, blk},
        {blk, blk, blk, red, blk},
        {blk, blk, blk, red, blk}};
    imprimir_desenho(frame, pio, sm);
  }
  else if (key == '8')
  {
    Matriz_leds_config frame = {
        {blk, red, red, red, blk},
        {blk, red, blk, red, blk},
        {blk, red, red, red, blk},
        {blk, red, blk, red, blk},
        {blk, red, red, red, blk}};
    imprimir_desenho(frame, pio, sm);
  }
  else if (key == '9')
  {
    Matriz_leds_config frame = {
        {blk, red, red, red, blk},
        {blk, red, blk, red, blk},
        {blk, red, red, red, blk},
        {blk, blk, blk, red, blk},
        {blk, blk, blk, red, blk}};
    imprimir_desenho(frame, pio, sm);
  }
  else if (key == '0')
  {
    Matriz_leds_config frame = {
        {blk, red, red, red, blk},
        {blk, red, blk, red, blk},
        {blk, red, blk, red, blk},
        {blk, red, blk, red, blk},
        {blk, red, red, red, blk}};
    imprimir_desenho(frame, pio, sm);
  }
}

char *on_off(bool on)
{
  if (on)
    return "^";
  else
    return "`";
}

void updateDisplay()
{
  printf("Atualizando display\n");
  ssd1306_fill(&ssd, false); // Limpa o display
  char buffer[15];
  char buffer2[14];
  sprintf(buffer, "Led: G:%s  B:%s", on_off(ledGreen), on_off(ledBlue));
  if (!charDigitado)
    charDigitado = ' ';
  sprintf(buffer2, "[%c] - U4C6O12T", charDigitado);
  ssd1306_draw_string(&ssd, buffer2, 8, 10);         // Desenha uma string
  ssd1306_draw_string(&ssd, buffer, 10, 25);         // Desenha uma string
  ssd1306_draw_string(&ssd, "Aluno Luinix", 15, 48); // Desenha uma string
  ssd1306_send_data(&ssd);                           // Atualiza o display
}

void button_irq_handler(uint gpio, uint32_t events)
{
  uint32_t current_time = to_ms_since_boot(get_absolute_time());
  if (current_time - tempo_botao_pressionado < DEBOUNCE_TIME)
    return;

  if (gpio == BUTTON_A)
  {
    printf("Botão A\n");
    gpio_put(LED_GREEN, ledGreen = !ledGreen);
  }
  else
  {
    gpio_put(LED_BLUE, ledBlue = !ledBlue);
    printf("Botão B\n");
  }
  updateDisplay();
  tempo_botao_pressionado = current_time;
}

void updateMatriz(char charDigitado, PIO pio, uint sm)
{
  if (charDigitado >= '0' && charDigitado <= '9')
  {
    actionMatriz(charDigitado, pio, sm);
  }
  else
  {
    actionMatriz(' ', pio, sm);
  }
}

int main()
{
  setlocale(LC_ALL, "Portuguese_Brazil");

  stdio_init_all();
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);

  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
  ssd1306_config(&ssd);
  ssd1306_send_data(&ssd);

  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  gpio_init(BUTTON_A);
  gpio_init(BUTTON_B);
  gpio_set_dir(BUTTON_A, GPIO_IN);
  gpio_set_dir(BUTTON_B, GPIO_IN);
  gpio_pull_up(BUTTON_A);
  gpio_pull_up(BUTTON_B);

  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
  gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

  gpio_init(LED_BLUE);
  gpio_init(LED_GREEN);
  gpio_set_dir(LED_BLUE, GPIO_OUT);
  gpio_set_dir(LED_GREEN, GPIO_OUT);
  gpio_put(LED_BLUE, 0);
  gpio_put(LED_GREEN, 0);

  PIO pio = pio0;
  uint sm = configurar_matriz(pio);
  actionMatriz(' ', pio, sm);

  printf("Bem vindo a Atividade U4C6O12T!\n");
  updateDisplay();

  while (true)
  {
    if (scanf(" %c", &charDigitado))
    {
      updateMatriz(charDigitado, pio, sm);
      updateDisplay();
    }
    sleep_ms(200);
  }
}
