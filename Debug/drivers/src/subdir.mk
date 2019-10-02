################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/src/stm32f103c8t6_gpio.c \
../drivers/src/stm32f103c8t6_interrupt.c \
../drivers/src/stm32f103c8t6_rcc.c \
../drivers/src/stm32f103c8t6_timer.c 

OBJS += \
./drivers/src/stm32f103c8t6_gpio.o \
./drivers/src/stm32f103c8t6_interrupt.o \
./drivers/src/stm32f103c8t6_rcc.o \
./drivers/src/stm32f103c8t6_timer.o 

C_DEPS += \
./drivers/src/stm32f103c8t6_gpio.d \
./drivers/src/stm32f103c8t6_interrupt.d \
./drivers/src/stm32f103c8t6_rcc.d \
./drivers/src/stm32f103c8t6_timer.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/src/%.o: ../drivers/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -I"C:/Users/Pawel/Documents/ST/STM32F103/Counter_of_area/drivers/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


