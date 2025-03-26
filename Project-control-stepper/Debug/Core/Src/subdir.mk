################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/command.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/stepper.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/vl53l0x_api.c \
../Core/Src/vl53l0x_api_calibration.c \
../Core/Src/vl53l0x_api_core.c \
../Core/Src/vl53l0x_api_ranging.c \
../Core/Src/vl53l0x_api_strings.c \
../Core/Src/vl53l0x_platform.c \
../Core/Src/vl53l0x_platform_log.c 

OBJS += \
./Core/Src/command.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/stepper.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/vl53l0x_api.o \
./Core/Src/vl53l0x_api_calibration.o \
./Core/Src/vl53l0x_api_core.o \
./Core/Src/vl53l0x_api_ranging.o \
./Core/Src/vl53l0x_api_strings.o \
./Core/Src/vl53l0x_platform.o \
./Core/Src/vl53l0x_platform_log.o 

C_DEPS += \
./Core/Src/command.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/stepper.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/vl53l0x_api.d \
./Core/Src/vl53l0x_api_calibration.d \
./Core/Src/vl53l0x_api_core.d \
./Core/Src/vl53l0x_api_ranging.d \
./Core/Src/vl53l0x_api_strings.d \
./Core/Src/vl53l0x_platform.d \
./Core/Src/vl53l0x_platform_log.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/command.cyclo ./Core/Src/command.d ./Core/Src/command.o ./Core/Src/command.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stepper.cyclo ./Core/Src/stepper.d ./Core/Src/stepper.o ./Core/Src/stepper.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/vl53l0x_api.cyclo ./Core/Src/vl53l0x_api.d ./Core/Src/vl53l0x_api.o ./Core/Src/vl53l0x_api.su ./Core/Src/vl53l0x_api_calibration.cyclo ./Core/Src/vl53l0x_api_calibration.d ./Core/Src/vl53l0x_api_calibration.o ./Core/Src/vl53l0x_api_calibration.su ./Core/Src/vl53l0x_api_core.cyclo ./Core/Src/vl53l0x_api_core.d ./Core/Src/vl53l0x_api_core.o ./Core/Src/vl53l0x_api_core.su ./Core/Src/vl53l0x_api_ranging.cyclo ./Core/Src/vl53l0x_api_ranging.d ./Core/Src/vl53l0x_api_ranging.o ./Core/Src/vl53l0x_api_ranging.su ./Core/Src/vl53l0x_api_strings.cyclo ./Core/Src/vl53l0x_api_strings.d ./Core/Src/vl53l0x_api_strings.o ./Core/Src/vl53l0x_api_strings.su ./Core/Src/vl53l0x_platform.cyclo ./Core/Src/vl53l0x_platform.d ./Core/Src/vl53l0x_platform.o ./Core/Src/vl53l0x_platform.su ./Core/Src/vl53l0x_platform_log.cyclo ./Core/Src/vl53l0x_platform_log.d ./Core/Src/vl53l0x_platform_log.o ./Core/Src/vl53l0x_platform_log.su

.PHONY: clean-Core-2f-Src

