
DRIVERS_SOURCE ?= ./Keil.STM32F4xx_DFP.2.13.0/Drivers
DRIVERS_DESTINATION ?= ./Sources/CSTM32F4

COPYMAP = ./Sources/CSTM32F4/include/map.txt


install:
	@test ! -f $(COPYMAP) || (echo "already installed, run uninstall first" ; exit 1)
	mkdir -p $(DRIVERS_DESTINATION)/hal
	cp -r $(DRIVERS_SOURCE)/STM32F4xx_HAL_Driver/Src/* ./Sources/CSTM32F4/hal
	cp -rv $(DRIVERS_SOURCE)/STM32F4xx_HAL_Driver/Inc/* ./Sources/CSTM32F4/include >> $(COPYMAP)
	cp -rv $(DRIVERS_SOURCE)/CMSIS/Device/ST/STM32F4xx/Include/* ./Sources/CSTM32F4/include >> $(COPYMAP)
	cp -rv ./Sources/CSTM32F4/stm32f4xx_hal_conf.h ./Sources/CSTM32F4/include >> $(COPYMAP)
	cp -rv ./CMSIS_5/CMSIS/Core/Include/* ./Sources/CSTM32F4/include >> $(COPYMAP)

uninstall:
	@test -f $(COPYMAP) || (echo "not installed" ; exit 1)
	rm -rf ./Sources/CSTM32F4/hal
	awk 'BEGIN {FS=" -> "} { print $$2 }' $(COPYMAP) | xargs rm -rf
	rm $(COPYMAP)


.PHONY: install uninstall
