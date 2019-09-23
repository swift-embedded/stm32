// swift-tools-version:5.1
import PackageDescription

var cSettings: [CSetting] = [
    .headerSearchPath("../../CMSIS_5/CMSIS/Core/Include"),
    .headerSearchPath("../../Keil.STM32F4xx_DFP.2.13.0/CMSIS/Driver"),
    .headerSearchPath("../../Keil.STM32F4xx_DFP.2.13.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include"),
    .headerSearchPath("../../Keil.STM32F4xx_DFP.2.13.0/Drivers/STM32F4xx_HAL_Driver/Inc"),
]

let package = Package(
    name: "STM32F4",
    products: [
        .library(
            name: "STM32F4Startup",
            targets: ["STM32F4Startup"]
        ),
        .library(
            name: "STM32F4",
            targets: ["STM32F4"]
        ),
        .library(
            name: "CSTM32F4",
            targets: ["CSTM32F4"]
        ),
    ],
    dependencies: [
        .package(url: "https://github.com/swift-embedded/hardware", .branch("master")),
        .package(url: "https://github.com/swift-embedded/crt0", .branch("master")),
    ],
    targets: [
        .target(
            name: "STM32F4Startup",
            dependencies: ["Crt0"],
            cSettings: cSettings + [
                .define("__STARTUP_CLEAR_BSS"),
                .define("__STARTUP_COPY_MULTIPLE"),
                .define("__STACK_SIZE", to: "0x6000"),
                .define("__HEAP_SIZE", to: "0x6000"),
            ]
        ),
        .target(
            name: "CSTM32F4",
            sources: [
                "./extensions",
                "./hal/stm32f4xx_hal.c",
                "./hal/stm32f4xx_hal_gpio.c",
                "./hal/stm32f4xx_hal_rcc.c",
                "./hal/stm32f4xx_hal_cortex.c",
                "./hal/stm32f4xx_hal_i2c.c",
                "./hal/stm32f4xx_hal_i2c_ex.c",
                "./hal/stm32f4xx_hal_dma.c",
                "./hal/stm32f4xx_hal_pwr_ex.c",
                "./hal/stm32f4xx_hal_spi.c",
                "./hal/stm32f4xx_ll_i2c.c",
            ],
            cSettings: cSettings + [.headerSearchPath(".")]
        ),
        .target(
            name: "STM32F4",
            dependencies: ["CSTM32F4", "Hardware"],
            cSettings: cSettings
        ),
    ]
)
