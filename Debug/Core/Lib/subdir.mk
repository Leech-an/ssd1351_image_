################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/fonts.c \
../Core/Lib/ssd1351.c 

OBJS += \
./Core/Lib/fonts.o \
./Core/Lib/ssd1351.o 

C_DEPS += \
./Core/Lib/fonts.d \
./Core/Lib/ssd1351.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/%.o Core/Lib/%.su Core/Lib/%.cyclo: ../Core/Lib/%.c Core/Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Lib

clean-Core-2f-Lib:
	-$(RM) ./Core/Lib/fonts.cyclo ./Core/Lib/fonts.d ./Core/Lib/fonts.o ./Core/Lib/fonts.su ./Core/Lib/ssd1351.cyclo ./Core/Lib/ssd1351.d ./Core/Lib/ssd1351.o ./Core/Lib/ssd1351.su

.PHONY: clean-Core-2f-Lib

