################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Src/Startup/startup_stm32l073rztx.s 

OBJS += \
./Core/Src/Startup/startup_stm32l073rztx.o 

S_DEPS += \
./Core/Src/Startup/startup_stm32l073rztx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Startup/%.o: ../Core/Src/Startup/%.s Core/Src/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-Src-2f-Startup

clean-Core-2f-Src-2f-Startup:
	-$(RM) ./Core/Src/Startup/startup_stm32l073rztx.d ./Core/Src/Startup/startup_stm32l073rztx.o

.PHONY: clean-Core-2f-Src-2f-Startup

