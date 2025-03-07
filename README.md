# orientation-week8-congestion-map

![GitHub repo size](https://img.shields.io/github/repo-size/ysnanako/orientation-week8-congestion-map)
![GitHub last commit](https://img.shields.io/github/last-commit/ysnanako/orientation-week8-congestion-map)

This project is part of the **National Cheng Kung University (NCKU) - Smart Electronic Design Automation Laboratory (SEDA)** summer training program, focusing on **Congestion Map generation for circuit routing evaluation**.

## 📖 Table of Contents

- [Project Overview](#project-overview)
- [Input Format](#input-format)
- [Output Format](#output-format)
- [Project Structure](#project-structure)
- [Congestion Map Computation Flow](#congestion-map-computation-flow)
- [Example Execution](#example-execution)
- [Generated Plots](#generated-plots)
- [Contribution Guide](#contribution-guide)
- [Contact Information](#contact-information)

## 📝 Project Overview

This project implements **Congestion Map generation** to evaluate the **routability of a circuit placement**. The process involves:
1. **Partitioning the placement region into uniform G-cells.**
2. **Using FLUTE to decompose multi-pin nets into 2-pin nets.**
3. **Calculating the demand and supply for each routing edge.**
4. **Identifying congested areas where demand exceeds supply.**
5. **Generating visual representations of congestion levels.**

### **Key Features:**
- **Parses placement files (`.aux`, `.nodes`, `.nets`, `.pl`, `.scl`, `.route`).**
- **Uses FLUTE to construct Rectilinear Steiner Minimum Trees (RSMT).**
- **Computes routing demand and supply for each G-cell.**
- **Identifies top 10% most congested G-cells.**
- **Outputs congestion maps as `.dpx` visualization files.**

## 📄 Input Format

The program takes the following input files:
- **.aux** - Entry point linking to other files.
- **.nodes** - Defines circuit components.
- **.nets** - Describes netlist connectivity.
- **.pl** - Specifies initial placement coordinates.
- **.scl** - Defines row placement structure.
- **.route** - Contains routing information (tile size, layers, spacing).

📄 **Example .route File**
```
Grid    100 100 6
MinWireWidth    1 1 1 1 1 1
MinWireSpacing  2 2 2 2 2 2
GridOrigin      0 0
TileSize        10 10
```

## 📄 Output Format

The program generates congestion visualization files:
- **`circuit_ver.dpx`** - Congestion map for vertical edges.
- **`circuit_hor.dpx`** - Congestion map for horizontal edges.
- **`circuit_w_ver.dpx`** - Wire visualization for vertical edges.
- **`circuit_w_hor.dpx`** - Wire visualization for horizontal edges.

📄 **Example circuit_ver.dpx**
```
COLOR pink
SRF 10 20 30 40
COLOR blue
SRF 50 60 70 80
...
```

## 🧰 Project Structure

```
📂 orientation-week8-congestion-map/
│── 📂 flute-3.1/ # (ignored)  
│── 📂 obj/ # (ignored)    
│── 📂 src/ 
│   ├── main.cpp  
│   ├── parser.cpp  
│   ├── alg.cpp  
│── 📄 circuit.aux # (ignored)  
│── 📄 circuit_ver.dpx # (ignored)  
│── 📄 circuit_hor.dpx # (ignored)  
│── 📄 circuit_w_ver.dpx # (ignored)   
│── 📄 circuit_w_hor.dpx # (ignored)  
│── 🔧 Makefile  
│── 📜 README.md # This file  
│── 📜 .gitignore  
```

## 🔹 **Congestion Map Computation Flow**

### **1. Read Input Files**
- Parses `.aux`, `.nodes`, `.nets`, `.pl`, `.scl`, `.route` files.

### **2. Generate Steiner Trees using FLUTE**
- Uses **FLUTE** to decompose **multi-pin nets into 2-pin nets**.

### **3. Compute Routing Demand & Supply**
- **Demand Calculation**: Counts wires crossing each routing edge.
- **Supply Calculation**: Determined by layer widths and spacing constraints.

### **4. Compute Overflow and Identify Congested Areas**
- **Overflow = Demand - Supply**.
- Marks **top 10% most congested G-cells**.

### **5. Generate Congestion Map Visualizations**
- Saves congestion levels in `.dpx` format for visualization.

## ⚡ **Example Execution**

```bash
make
./FLUTE_test circuit.aux circuit_ver.dpx circuit_hor.dpx circuit_w_ver.dpx circuit_w_hor.dpx
./display.x circuit.dpx
```

## 🖼️ Generated Plots

Below are the generated plots from the `display.x` output:

**congestion map ver**  
![ver](https://github.com/user-attachments/assets/fd16f4aa-4abf-4e47-afb4-e83baec39dd5)  
**congestion map ver**  
![hor](https://github.com/user-attachments/assets/554f4b3e-9374-4efb-907c-535ed44b00a1)  
**wire ver**  
![w_ver](https://github.com/user-attachments/assets/c6b6aeae-2b25-4ad0-bbd3-65066ccf48ff)  
**wire hor**  
![w_hor](https://github.com/user-attachments/assets/81d76dc4-3464-4f33-b84c-4a40b5965c67)  

## 🤝 Contribution Guide

1. Fork this repository.
2. Create a new branch (`git checkout -b feature-xyz`).
3. Commit your changes (`git commit -m 'Add new feature'`).
4. Push to the remote branch (`git push origin feature-xyz`).
5. Submit a Pull Request.

## 📬 Contact Information

- 📧 Email: [m16131056@gs.ncku.edu.tw](mailto:m16131056@gs.ncku.edu.tw)
- 🌎 University: [National Cheng Kung University (NCKU)](https://www.ncku.edu.tw)
- 📖 Course: Summer Training Program - Congestion Map Analysis
