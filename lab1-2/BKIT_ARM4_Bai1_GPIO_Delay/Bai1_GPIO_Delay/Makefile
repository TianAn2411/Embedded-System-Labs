# Build without STM32CubeIDE: `make`, `make flash`
TARGET  = Bai1_GPIO_Delay
GCC_DIR ?= $(HOME)/opt/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin
OPENOCD ?= $(HOME)/opt/xpack-openocd-0.12.0-7/bin/openocd
PREFIX  = $(GCC_DIR)/arm-none-eabi-
CC      = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE    = $(PREFIX)size
BUILD   = build

SRCS = $(wildcard Core/Src/*.c) $(wildcard Drivers/STM32F4xx_HAL_Driver/Src/*.c) Core/Startup/startup_stm32f407zetx.s
OBJS = $(addprefix $(BUILD)/,$(addsuffix .o,$(basename $(SRCS))))

MCU    = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS = $(MCU) -DUSE_HAL_DRIVER -DSTM32F407xx -DDEBUG -g3 -O0 -Wall \
         -ffunction-sections -fdata-sections -MMD -MP \
         -ICore/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
         -IDrivers/CMSIS/Device/ST/STM32F4xx/Include -IDrivers/CMSIS/Include
LDFLAGS = $(MCU) -TSTM32F407ZGTX_FLASH.ld --specs=nano.specs --specs=nosys.specs \
          -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map -Wl,--no-warn-rwx-segments

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin $(BUILD)/$(TARGET).hex

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(MCU) -x assembler-with-cpp -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/$(TARGET).hex: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

flash: $(BUILD)/$(TARGET).elf
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $< verify reset exit"

clean:
	rm -rf $(BUILD)

-include $(OBJS:.o=.d)
.PHONY: all flash clean
