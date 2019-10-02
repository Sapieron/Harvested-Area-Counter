# Harvested Area Counter - an STM32F103C8T6 ,,Bluepill" based project
This application uses STM32F103C8T6 development board ,,bluepill" and is used to show area of crop that has been harvested. Other peripherals like 7-segment display are mounted on a custom designed mainboard (design files also available within this repository).

## Short Brief
A magnet is mounted on a wheel of a harvester, and as it spins the change of magnetic field is read by three hall sensors directly connected to the mainboard. Only correct order of switching these sensors results in increasing the number shown on 7-segment display. Main idea that stands behind this project is to connect both programming and electronics design skills, focused on ARM architecture microcontrolers. Project's hardware, as well as sofware and firmware, were all created from scratch.

## Technologies used
For this project following technologies, programs and hardware was used:

* C (embedded C11)
* Altium Designer 18.1.7
* Eclipse IDE with GCC compiler
* ST-Link V2.1 programmer/debugger
* STM32F103C8T6 ,,Bluepill" development board
* Custom hardware
## Setup
### Software Setup
Depending on used setup, number of digits on a 7-segment display may differ. In main.c modify `NUM_OF_DIGITS_ON_DISPLAY` number to fit you setup. Also, to meet different diamater of a wheel and a hervester's header width, in main.c modify `ONE_SPIN` to determine how much area has been harvested with one full spin of a wheel. It can be caluculated with a simple equation used to calculate cylinder field -` 2 * π * r * h`, where `2πr` stands for circumference of a wheel and `h` stands for the width of harvester's header.

### Hardware setup and flashing memory
Main Control Unit's memory may be flashed in several ways:

* To use inbuilt Mini USB programmer to flash software, please refer to Bluepill Project's homepage
* Use SWD connector to flash the board with an external programmer. Pinouts are available under this [link](https://user-images.githubusercontent.com/26856618/33534204-dcfa66be-d85a-11e7-8e1f-fdb426510126.gif). Next, connect your hardware programmer to bluepill board and flash the `.elf` file (how to do that depends on IDE or software flashing software used - please refer to tutarials specific to your setup)
## Inspiration
To create hardware, sofware and firmware from scratch - at start I found it easy, but time consuming. My initial idea was right just with the second point. Underneath are the books, that helped my to go through this project:

* Robert C. Martin ,,Clean Code" - main inspiration, main source of knowledge;
* James Grenning ,,Test-Driven Development for Embedded C" - even though no test framework was used in this project I still found this book really usefull and inspiring. Looking forward to finish it soon and start a project with ideas that stand behind TDD. 
## Contact
Created by Paweł Klisz @ pawelochojec@gmail.com- feel free to contact me!
