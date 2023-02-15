# Trabalho Final Da Disciplina FSE

## Alunos
|Matrícula | Aluno |
| -- | -- |
| 16/0119006  |  Enzo Gabriel Guedes Queiroz Saraiva |
| 18/0054554  |  Paulo Batista da Cruz Junior |
| 18/0011308  |  Peniel Etèmana Désirez-Jésus Zannoukou |


## Objetivo
O objetivo deste trabalho é criar sensores e atuadores distribuídos baseados nos microcontroladores ESP32 interconectados via Wifi através do protocolo MQTT, 
podendo ser aplicada em diversos contextos de automação a exemplo das áreas de Automação Residencial, Predial ou Industrial.

Os microcontroladores ESP32 irão controlar a aquisição de dados de sensores, botões e chaves e acionar saídas como leds, dentre outras. Haverão dois modos de operação dos dispositivos: modo energia que representa o dispositivo ligado à tomada e modo bateria que representa o dispositivo que deverá operar em modo de baixo consumo.

Toda a comunicação foi feita via rede Wifi com o protocolo MQTT e foi provido um servidor central para cadastro e controle dos dispositivos através da plataforma Thingsboard.


## Dispositivo montado na plataforma Thingsboard pelo grupo


<a href="https://ibb.co/5rKnQ68"><img src="https://i.ibb.co/rxdcBm6/captura-Tela-Dashboards2023-02-14-221355.png" alt="captura-Tela-Dashboards2023-02-14-221355" border="0" /></a>



## Objetivos

O objetivo deste trabalho é criar sensores e atuadores distribuídos baseados nos microcontroladores ESP32 interconectados via Wifi através do protocolo MQTT, podendo ser aplicada em diversos contextos de automação a exemplo das áreas de Automação Residencial, Predial ou Industrial.

Os microcontroladores ESP32 irão controlar a aquisição de dados de sensores, botões e chaves e acionar saídas como leds, dentre outras. Haverão dois modos de operação dos dispositivos: modo energia que representa o dispositivo ligado à tomada e modo bateria que representa o dispositivo que deverá operar em modo de baixo consumo.

Toda a comunicação será feita via rede Wifi com o protocolo MQTT e será provido um servidor central para cadastro e controle dos dispositivos através da plataforma Thingsboard.


## Recursos utilizados

- PlatformIO

## Utilização

Baixe o repositorio e direcione-se até a pasta esp32:

```
$ git clone https://github.com/higton/FSE.git
```

```
$ cd esp32/
```

Após isso, use a extensão do platformIO para o vscode abrindo  o projeto
e um novo terminal.
Nesse terminal insira o comando 

/esp32 $ pio run -t menuconfig
para acessar a tela de configurações
na tela de configuração
procure pela opção 
Project Configuration
 e em seguida 
Wifi Configuration
 

Lá dentro é necessario inserir o SSID e a senha da rede wifi
Após a configuração da Wifi, faça o upload do codigo para a placa utilizando o platformIO

Finalizado o upload, o projeto pode ser executado através do botão Serial Monitor.


## Vídeo 
[link do vídeo](https://youtu.be/5H9Y6UxDUOA)
