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
            // Provides all the necessary things to run a Swift program
            // on an stm32f4 devices, including an high-level HAL library.
            name: "STM32F4",
            targets: ["STM32F4", "STM32F4Startup"]
        ),
        .library(
            // Minimal library providing only the necessary symbols to
            // start a swift program on an stm32f4 device.
            name: "STM32F4Startup",
            targets: ["STM32F4Startup"]
        ),
        .library(
            // The C STM32F4 HAL Library
            name: "CSTM32F4",
            targets: ["CSTM32F4"]
        ),
    ],
    dependencies: [
        .package(url: "https://github.com/swift-embedded/unicode-support", .branch("master")),
        .package(url: "https://github.com/swift-embedded/hardware", .branch("master")),
        .package(url: "https://github.com/swift-embedded/crt0", .branch("master")),
        // just so packages depending on this one can use `swift run cross ...` without
        // explicitly depending on it
        .package(url: "https://github.com/swift-embedded/cross", .branch("master")),
    ],
    targets: [
        .target(
            name: "STM32F4",
            dependencies: ["CSTM32F4", "Hardware"],
            cSettings: cSettings
        ),
        .target(
            name: "STM32F4Startup",
            dependencies: ["Crt0", "SimpleUnicodeSupport"],
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
                "./hal/stm32f4xx_hal_uart.c",
                "./hal/stm32f4xx_ll_i2c.c",
            ],
            cSettings: cSettings + [.headerSearchPath(".")]
        ),
    ]
)
