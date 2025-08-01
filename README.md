# Lower Limb Exoskeleton with Gait Control System

## 🦾 Project Overview

This project presents the design and development of an **underactuated lower limb exoskeleton** capable of augmenting human strength by **400%** while maintaining natural gait patterns. Unlike traditional 6+ DOF systems, our innovative 4-DOF approach (hip and knee joints only) provides effective power augmentation at reduced complexity and cost.

### Key Achievements
- ✅ **400% strength augmentation** validated through EMG testing
- ✅ **5x peak strength amplification** capability
- ✅ **Real-time gait pattern tracking** and control
- ✅ **Novel underactuated approach** with multi-point ground contact
- ✅ **Custom control architecture** with motion disturbance rejection

---

## 🎯 Project Objectives

1. **Motor Control**: Configure torque and position controllers for BLDC motors
2. **Gait Analysis**: Analyze differences in gait patterns with/without load and exoskeleton
3. **Controller Development**: Implement PD + Bang-Bang controller for desired gait pattern tracing
4. **Strength Validation**: Test controller efficacy in augmenting user strength through force analysis
5. **EMG Validation**: Quantify strength amplification using electromyography data

---

## 🛠️ Technical Architecture

### Hardware Components
- **Motors**: BLDC motors with torque/position control
- **Sensors**: Inertial Measurement Unit (IMU) for feedback
- **Control**: Custom microcontroller unit with motor drivers
- **Power**: Battery system for portable operation
- **Mechanical**: 4-DOF exoskeleton frame (hip and knee joints)

### Control System
```
Setpoint Generation → PD Controller → Bang-Bang Control → Motor Actuation
        ↑                                                        ↓
    Gait Pattern                                            IMU Feedback
```

### Software Architecture
- **Angle Update Module**: Real-time joint angle monitoring
- **Controller Module**: PD control with dynamic motion disturbance rejection
- **Actuation Module**: Speed-to-duty cycle mapping with threshold-based system control

---

## 🔬 Methodology

### 1. Hardware Setup & Integration
- Motor and driver configuration
- IMU calibration and mounting
- Battery and power management system
- Mechanical assembly and testing

### 2. Control Algorithm Development
- **Position Controller**: Single DOF motor position control
- **Gait Generation**: Algorithm for natural walking pattern synthesis
- **PD + Bang-Bang Controller**: Hybrid approach for robust tracking
- **Motion Compensation**: Dynamic disturbance rejection system

### 3. Validation & Testing
- **Single leg gait cycle testing**
- **Controller response analysis**
- **EMG data acquisition and analysis**
- **Strength augmentation quantification**

---

## 📊 Results

### Control Performance
- Successfully achieved repeating gait cycles
- Stable position control with measured time-domain response
- Effective motion disturbance rejection

### Strength Augmentation Validation
**EMG Analysis Results:**
- **Without Support**: Max amplitude ~0.8mV (burst activity)
- **With Exoskeleton**: Max amplitude ~4mV with continuous 1mV baseline
- **Conclusion**: 5x peak strength amplification, 400% overall augmentation

### Key Findings
1. **Multi-core Processing**: Parallel control for multiple DOF systems recommended
2. **Speed vs. Torque Control**: Current-based control approaches preferred for robustness
3. **Impedance Control**: Direct torque input more effective than speed-based control
4. **Validation Success**: Low-cost prototype successfully augments user strength

---

## 🏗️ System Architecture

### Kinematic Model
```
Hip Joint (θh, ωh) → Thigh Link (lt) → Knee Joint (θk, ωk) → Shank Link (ls) → Ankle
```

### Compensation Algorithm
For undisturbed shank angular position during thigh motion:
```
θ̇k = -ωh (compensation velocity for knee actuator)
```

### Control Equations
- **Position Control**: PD controller with setpoint lookup table
- **Motion Compensation**: Dynamic adjustment based on hip joint velocity
- **Actuation Mapping**: Speed-to-duty cycle conversion with threshold control

---

## 🔧 Implementation Details

### PCB Design
- Custom circuit board for motor control integration
- IMU interface and data acquisition
- Power management and battery monitoring

---

### Key Functions
- **Angle Update**: Real-time joint angle monitoring via IMU
- **Setpoint Lookup**: Gait pattern reference generation
- **Error Calculation**: Position error computation for control
- **Speed Mapping**: Controller output to motor speed conversion
- **Threshold Control**: Safety and performance optimization

---

## 📈 Performance Metrics

| Metric | Value | Validation Method |
|--------|--------|------------------|
| Strength Augmentation | 400% | EMG Analysis |
| Peak Strength Amplification | 5x | EMG Peak Comparison |
| Control DOF | 4 (Hip + Knee) | System Design |
| Response Time | Real-time | Controller Testing |
| Ground Contact | Multi-point | Mechanical Design |

---

## 🚀 Future Work

### Immediate Improvements
- **Multi-core Implementation**: Parallel processing for enhanced performance
- **Torque-based Control**: Direct current control for improved robustness
- **Impedance Control**: Enhanced human-robot interaction
- **Response Tuning**: Further PD parameter optimization

### Advanced Features
- **Adaptive Control**: Machine learning-based gait adaptation
- **Wireless Communication**: Remote monitoring and control
- **Energy Harvesting**: Self-sustaining power systems
- **Full-body Integration**: Extension to upper limb assistance

---

## 👥 Team

**Project Members:**
- Adithya Venkata Narayanan – 111120001
- R Charan Bhardhwaj – 111120109

**Project Guides:**
- Dr. N Siva Shanmugam (Internal Guide)
- Dr. D Ezhilarasi (Co-Guide)

---

## 📚 Key References

1. **Kinoshita, H. (1985)**. Effects of different loads and carrying systems on selected biomechanical parameters describing walking gait. *Ergonomics, 28(9), 1347–1362.*

2. **Al-Shuka, et al. (2019)**. Biomechanics, actuation, and multi-level control strategies of power-augmentation lower extremity exoskeletons: an overview. *International Journal of Dynamics and Control, 7(4), 1462–1488.*

3. **Grimmer, et al. (2020)**. Human Lower Limb Joint Biomechanics in Daily Life Activities: A Literature Based Requirement Analysis for Anthropomorphic Robot Design. *Frontiers in Robotics and AI, 7.*

---

## 🏆 Project Impact

This research addresses a critical gap in exoskeleton technology by demonstrating that **underactuated systems** can provide effective power augmentation comparable to traditional higher-DOF systems. The work contributes to:

- **Cost-effective Design**: Reduced complexity while maintaining performance
- **Novel Control Approach**: PD + Bang-Bang hybrid controller
- **Validated Performance**: Quantified strength augmentation through EMG
- **Real-world Application**: Practical solution for load-bearing augmentation

---

## 📄 License

This project is part of academic research conducted at National Institute of Technology, Tiruchirapalli. Please cite appropriately if using this work for research purposes.

*This project represents a significant step forward in making exoskeleton technology more accessible and practical for real-world applications.*