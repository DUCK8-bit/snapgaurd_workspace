import serial
import time
import json
import threading

class ArduinoLink:
    def __init__(self, port='COM3', baudrate=115200):
        self.ser = serial.serial_for_url('rfc2217://127.0.0.1:4000', baudrate=115200)
        self.telemetry = {"temp": 25.0, "vibration": 0.0}
        self.running = True
        
        # Start a daemon thread to constantly read sensor data
        self.thread = threading.Thread(target=self._read_loop, daemon=True)
        self.thread.start()

    def _read_loop(self):
        while self.running:
            try:
                line = self.ser.readline().decode('utf-8').strip()
                if line.startswith("{"):
                    self.telemetry = json.loads(line)
            except Exception as e:
                pass
            time.sleep(0.01)

    def trigger_estop(self):
        """Sends command to Arduino to trip the physical relay and buzzer"""
        self.ser.write(b"ESTOP\n")
        
    def reset_system(self):
        self.ser.write(b"RESET\n")

    def get_latest_telemetry(self):
        return self.telemetry

    def close(self):
        self.running = False
        self.ser.close()
