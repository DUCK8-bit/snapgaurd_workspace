# SnapGuard: Adaptive Edge Safety Intelligence
**Comprehensive Project Dossier & Presentation Source Material**

*This document is formatted for ingestion by NotebookLM, ChatGPT, or other AI assistants to generate slide decks, pitch scripts, and technical Q&A preparation.*

---

## 1. Project Overview & Elevator Pitch
**The Pitch:** SnapGuard is a multimodal, cyber-physical safety system that runs entirely on the edge. Powered by the Snapdragon X Elite Hexagon NPU, it continuously analyzes visual and physical sensor data to assess risk, dynamically adapts its own AI workload to save power, and triggers immediate physical safety protocols—all without requiring an internet connection.

**The Ultimate Goal:** To prove that true industrial safety requires continuous, privacy-preserving, low-latency edge AI, and that the Snapdragon NPU is the optimal hardware to deliver it.

---

## 2. The Core Problem: Why Cloud AI Fails in Industrial Safety
Currently, AI safety systems rely heavily on cloud computing or power-hungry discrete GPUs. This introduces fatal flaws:
*   **Latency:** Round-trip network delays (Camera → Cloud → Action) are unacceptable when milliseconds determine physical safety.
*   **Reliability:** Industrial environments often have poor or air-gapped network connectivity. A Wi-Fi drop shouldn't mean a safety failure.
*   **Privacy:** Streaming continuous factory footage to the cloud violates strict enterprise privacy and compliance protocols.
*   **Power & Thermal Constraints:** Discrete GPUs consume too much power and generate too much heat for embedded, continuous-monitoring edge devices.

---

## 3. The SnapGuard Solution (The Architecture)
SnapGuard solves these problems by keeping all intelligence on the device.

**System Components:**
1.  **Vision AI:** YOLOv8n object detection model, quantized to INT8, running on the Snapdragon Hexagon NPU.
2.  **Physical Telemetry (Arduino):** Real-time hardware sensors capturing Machine Temperature, Vibration, and Proximity.
3.  **Multimodal Risk Engine:** A fusion layer that combines visual data (e.g., "Worker in Restricted Zone") with physical data (e.g., "Abnormal Machine Vibration") to calculate a real-time Risk Score (0-100).
4.  **Physical Actuator:** Arduino-driven local hardware responses (LEDs, buzzers, machine shutdown relays) triggered instantly by the Risk Engine.

---

## 4. Key Differentiators (The "Winning" Features)

### A. Adaptive Edge Intelligence (Dynamic Compute)
SnapGuard does not blindly run AI at maximum capacity. It intelligently allocates compute resources based on real-time risk, demonstrating a mature understanding of edge power budgets.
*   **LOW RISK (Score 0-25):** Machine is idle. Inference throttled to 2 FPS. (Maximum Power Saving)
*   **ELEVATED RISK (Score 30-55):** Machine active or person nearby. Inference increased to 10 FPS.
*   **CRITICAL RISK (Score 70-100):** Worker in restricted zone + machine anomaly. Inference pushed to MAX FPS for instantaneous actuator response.

### B. The Decision Trace (Explainable AI)
AI shouldn't be a black box. SnapGuard provides a real-time "Decision Trace" explaining exactly *why* a physical action was taken:
*   `[X] Person Detected (+25)`
*   `[X] Restricted Zone Violation (+45)`
*   `[ ] Machine Anomaly (+0)`
*   **Total Risk Score: 70/100 -> HIGH RISK**

---

## 5. Benchmarks & Proof (The Qualcomm AI Hub Story)
SnapGuard isn't a theoretical prototype; it is backed by hard hardware profiling on physical Snapdragon silicon.

**The Workflow:**
We took the standard YOLOv8n PyTorch model, exported it to ONNX, cleaned the graph for strict edge compilation, and submitted it to the **Qualcomm AI Hub**. The model was quantized to INT8 and profiled on a physical Snapdragon X Elite CRD.

**Official Hardware Profiling Results (Snapdragon X Elite / Hexagon NPU):**
*   **NPU Inference Latency:** 6.61 ms
*   **Theoretical NPU Throughput:** ~151 FPS
*   **Peak Inference Memory:** ~42.4 MB

**System End-to-End (E2E) Latency Decomposition:**
To prove system-level optimization, we isolated the latency pipeline (using a multi-threaded architecture to bypass camera IO blocking):
1.  **Camera Buffer & IO:** ~1.0 ms (Optimized via background thread)
2.  **AI Pre/Post-processing (CPU):** ~12.0 ms
3.  **NPU Inference (Hexagon):** 6.61 ms
4.  **Sensor Fusion & Serial IO:** ~1.0 ms
*   **Total E2E Latency:** < 25 ms (Instantaneous physical reaction)

---

## 6. The "Why Snapdragon?" Defense (Q&A Preparation)

**Q: Why didn't you just use OpenAI or a Cloud API?**
**A:** "Because our application is continuous, latency-sensitive, and operates in environments where internet access is unreliable. If the Wi-Fi drops in a factory, the safety system cannot go blind. SnapGuard operates 100% offline."

**Q: Why use the NPU instead of just the CPU or a discrete GPU?**
**A:** "A discrete GPU is excellent for high-throughput batch compute, but SnapGuard is designed for *continuous, lightweight, power-efficient* inference. By shifting the 6.61ms vision workload to the Hexagon NPU, we free up the CPU to handle real-time sensor fusion, UI rendering, and Arduino telemetry without thermal throttling or massive battery drain."

---

## 7. The 2-Minute Demo Script (The Presentation Flow)

**0:00 - 0:20 (The Setup)**
*Action: Display the SnapGuard Dashboard. Physically disconnect the laptop's Wi-Fi.*
*Speaker:* "This is SnapGuard. It is running entirely locally on this machine. No cloud, no internet. True edge safety intelligence."

**0:20 - 0:45 (Normal State)**
*Action: Show an empty camera feed. Point to the Adaptive Compute panel.*
*Speaker:* "Currently, the machine is idle. SnapGuard intelligently throttles the NPU to 2 frames per second to conserve power. The Risk Score is low."

**0:45 - 1:15 (Elevated Risk - Multimodal Fusion)**
*Action: Have a person step into the camera frame but stay outside the red Restricted Zone.*
*Speaker:* "A worker enters the area. The system detects them, but they are safe. However, let's look at the Arduino telemetry. The machine's vibration just spiked to abnormal levels."
*Action: Trigger vibration slider on Arduino simulator.*
*Speaker:* "Because the machine is now active, SnapGuard dynamically ramps the AI inference up to 10 FPS to monitor the situation more closely."

**1:15 - 1:45 (Critical Risk - Cyber-Physical Action)**
*Action: Person steps inside the red Restricted Zone on camera.*
*Speaker:* "The worker crosses into the restricted zone while the machine is experiencing anomalous vibrations. Our multimodal engine instantly calculates a Critical Risk Score."
*Action: Dashboard flashes RED. Arduino LEDs/Buzzer fire.*
*Speaker:* "The NPU shifts to MAX FPS, and in under 25 milliseconds, a command is sent to the Arduino to physically trigger the E-Stop, shutting down the machine before an accident occurs."

**1:45 - 2:00 (The Conclusion)**
*Speaker:* "Look at the telemetry: NPU inference took exactly 6.61 milliseconds. The intelligence stayed on the device, the decision happened locally, and the physical system responded instantly. That is the power of the Snapdragon Hexagon NPU."
