 # RGB Indicator Test

 This project demonstrates the usage of the `RGB_Indicator` class for controlling an RGB LED with various patterns and colors using an Arduino.

 ## Setup

 - Arduino board
 - RGB LED connected to pins 9, 10, and 11 (or modify the pins as needed)
 - Required libraries:
   - `FreeRTOS` for task management
   - `Arduino.h` for basic Arduino functions

 ## Files

 - `PDL_RGB_Indicator.h`: Header file containing the `RGB_Indicator` class and related structures.
 - `PDL_RGB_Indicator.cpp`: Implementation file for the `RGB_Indicator` class.
 - `Test_RGB_Indicator.ino`: Arduino sketch for testing the `RGB_Indicator` class.

 ## Installation

 1. Download the project files and place them in your Arduino library folder.
 2. Open `Test_RGB_Indicator.ino` in the Arduino IDE.
 3. Connect your Arduino board to your computer.
 4. Select the appropriate board and port in the Arduino IDE.
 5. Upload the sketch to your Arduino board.

 ## Usage

 The `Test_RGB_Indicator.ino` sketch will perform the following tests:

 - Test setting RGB values to various colors (red, green, blue, custom).
 - Test setting different predefined patterns (RED_BLINK, RAINBOW, OFF).
 - Test setting different ramp rates for color transitions.

 Observe the behavior of the RGB LED connected to your Arduino board to verify the functionality of the `RGB_Indicator` class.

 ## Customization

 - Modify the `TEST_PIN_R`, `TEST_PIN_G`, and `TEST_PIN_B` constants in `Test_RGB_Indicator.ino` to match your specific hardware setup.
 - Add or modify predefined patterns and colors in `PDL_RGB_Indicator.h` to suit your needs.

 ## License

 This project is licensed under the MIT License - see the LICENSE file for details.

 ## Acknowledgments

 - The `FreeRTOS` library for providing task management capabilities.
 - The Arduino community for their continuous support and contributions.

 ## Contributing

 Contributions are welcome! Please fork the repository and submit a pull request with your changes.

 ## Contact

 For any questions or suggestions, please open an issue in the GitHub repository.

