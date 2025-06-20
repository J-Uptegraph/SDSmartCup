# Smart Cup Project

An intelligent drinking cup system that monitors liquid volume, tilt angle, and controls flow rate using Arduino-based sensors and servo control.

## Features
- Monitoring liquid volume using Force Sensitive Resistors (FSR)
- Detecting cup tilt and preventing spills
- Controlling liquid flow rate through a servo-controlled valve
- Providing visual feedback through LED indicators
- Calculating complex fluid dynamics in real-time

### Required Components
- Arduino Nano 33 BLE Sense (with IMU)
- Servo motor (for valve control)
- Force Sensitive Resistors (FSR) - 2x for X and Y axis sensing
- LEDs - 8x total (4 green for volume, 4 red for tilt)
- Push buttons - 4x for flow control settings
- Resistors and connecting wires
- Custom cup with integrated sensors

### Pin Configuration
- **Servo Pin**: 10 (configurable to pin 11 for specific boards)
- **FSR Sensors**: A0 (X-axis/bottom), A7 (Y-axis/side)
- **Green LEDs** (Volume): Pins 2, 3, 4, 5
- **Red LEDs** (Tilt): Pins 6, 7, 8, 9
- **Buttons**: Pins 2, 4, 6, 8 (Off, Low, Medium, High)

## Software Architecture

### Core Classes

#### Cup Class
Models the physical cup geometry with parameters:
- Radius (Rc) and height (hc)
- Drinking aperture diameter and position
- Vent hole specifications
- Volume calculations and flow area methods

#### Liquid Class
Handles fluid dynamics calculations:
- Volume tracking and density properties
- Flow factor for different liquids (water, fiber drinks, etc.)
- Complex geometric calculations for liquid positioning
- Cubic and quartic equation solving for fluid shape

### Main Features

#### 1. Volume Monitoring (`18_02_2022.ino`)
- Real-time liquid level detection using bottom FSR
- 4-level volume indication (30%, 50%, 80%, Full)
- Green LED array for visual feedback

#### 2. Tilt Detection
- Side-mounted FSR detects cup tilting
- 4-level tilt warning system
- Red LED indicators for tilt severity
- Automatic spill prevention with servo activation

#### 3. Flow Control System
- Servo-controlled valve for flow rate management
- Vacuum pressure calculations for controlled dispensing
- Multiple flow rate settings (Off, Low, Medium, High)
- Intelligent spill prevention

#### 4. Button Interface
- ezButton library for debounced input handling
- Four control modes for different flow rates
- Real-time button state monitoring

<b>Check Out the Hardware Explaination Here:</b> [Demo Video](https://drive.google.com/file/d/1wOa7wEtdBXZI4i9xNtuNAEUCwSKBbSzh/view?usp=sharing)

## Code Structure

```
CupProject/
├── 18_02_2022/                    # Main sensor integration
├── Buttons/                       # Button control modules
│   ├── MultipleButtonCode/
│   └── SingleButtonCode/
├── ServoAttempt1/                 # Servo control and SmartCup library
├── ServoMotor/                    # Servo calibration
├── noWeight/                      # Simplified version without weight sensing
└── smartCupPrototype/             # Complete prototype implementation

Additional modules:
├── FSR/                           # Basic FSR testing
├── FSR_Gyroscope/                 # Gyroscope integration
└── FSR_Tilt_With_Servo_Control/   # Combined tilt and servo control
```

## Installation & Setup

### Dependencies
Install the following libraries in Arduino IDE:
- `ezButton` - For button debouncing
- `Arduino_LSM9DS1` - For IMU functionality
- `Servo` - For servo motor control

### Hardware Setup
1. Connect FSR sensors to analog pins A0 and A7
2. Wire LED arrays to digital pins 2-9
3. Connect servo to pin 10 (or 11 depending on board)
4. Install buttons on pins 2, 4, 6, 8 with pull-up resistors
5. Ensure IMU is properly initialized on Arduino Nano 33 BLE Sense

### Configuration
1. Calibrate FSR thresholds in the main code
2. Adjust servo open/close positions in `ServoValueCalibration.ino`
3. Set cup physical parameters in the cup constructor
4. Configure desired flow rates and control parameters

## Usage

### Basic Operation
1. Power on the Arduino
2. Fill cup with liquid - volume LEDs will indicate fill level
3. Tilt detection automatically activates warning LEDs
4. Use buttons to control flow rate settings
5. Servo automatically prevents spills when tilt threshold exceeded

### Flow Control Modes
- **Off**: No flow control active
- **Low**: Minimal flow rate (5 ml/sec default)
- **Medium**: Moderate controlled flow
- **High**: Maximum controlled flow rate

### Calibration
Run `ServoValueCalibration.ino` to find optimal servo positions:
- Press button during servo sweep to record positions
- Note open and closed positions for valve control
- Update `servoOpen` and `servoClose` variables accordingly

## Advanced Features

### Fluid Dynamics Modeling
The system includes sophisticated fluid dynamics calculations:
- Real-time liquid shape prediction based on tilt angle
- Vacuum pressure calculations for flow control
- Support for different liquid types (water, fiber drinks, etc.)
- Complex polynomial solving for accurate liquid positioning

### Sensor Fusion
Combines multiple sensor inputs:
- FSR pressure readings for volume and tilt
- IMU accelerometer data for precise angle measurement
- Time-based tracking for flow rate calculations

## Troubleshooting

### Common Issues
- **LEDs not responding**: Check pin connections and power supply
- **Servo not moving**: Verify servo power and signal connections
- **Inaccurate readings**: Recalibrate FSR thresholds and servo positions
- **Compilation errors**: Ensure all required libraries are installed

### Debug Mode
Enable serial output to monitor:
- FSR sensor readings
- Calculated angles and volumes
- Flow rate calculations
- Button press events

## Contributing

To contribute to this project:
1. Test new sensor configurations
2. Improve fluid dynamics algorithms
3. Add new control modes or features
4. Optimize power consumption
5. Enhance user interface

## License

This project is open source. Please refer to individual code files for specific licensing information.

## Acknowledgments

- Arduino community for sensor libraries
- Fluid dynamics algorithms based on engineering principles
- Hardware design inspired by practical drinking vessel requirements
