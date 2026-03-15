################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := 45_CMSIS_RTOS_osEventFlags
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
45_CMSIS_RTOS_osEventFlags.elf \

MAP_FILES += \
45_CMSIS_RTOS_osEventFlags.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
45_CMSIS_RTOS_osEventFlags.list \

OBJCOPY_HEX += \
45_CMSIS_RTOS_osEventFlags.hex \

OBJCOPY_BIN += \
45_CMSIS_RTOS_osEventFlags.bin \


# All Target
all: main-build

# Main-build Target
main-build: 45_CMSIS_RTOS_osEventFlags.elf secondary-outputs

# Tool invocations
45_CMSIS_RTOS_osEventFlags.elf 45_CMSIS_RTOS_osEventFlags.map: $(OBJS) $(USER_OBJS) C:\STM32f411\ FreeRTOS\ Workspace\45_CMSIS_RTOS_osEventFlags\STM32F411RETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "45_CMSIS_RTOS_osEventFlags.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\STM32f411 FreeRTOS Workspace\45_CMSIS_RTOS_osEventFlags\STM32F411RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="45_CMSIS_RTOS_osEventFlags.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

45_CMSIS_RTOS_osEventFlags.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "45_CMSIS_RTOS_osEventFlags.list"
	@echo 'Finished building: $@'
	@echo ' '

45_CMSIS_RTOS_osEventFlags.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "45_CMSIS_RTOS_osEventFlags.hex"
	@echo 'Finished building: $@'
	@echo ' '

45_CMSIS_RTOS_osEventFlags.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "45_CMSIS_RTOS_osEventFlags.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) 45_CMSIS_RTOS_osEventFlags.bin 45_CMSIS_RTOS_osEventFlags.elf 45_CMSIS_RTOS_osEventFlags.hex 45_CMSIS_RTOS_osEventFlags.list 45_CMSIS_RTOS_osEventFlags.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
