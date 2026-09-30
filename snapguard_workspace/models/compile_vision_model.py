import qai_hub as hub
from ultralytics import YOLO
import onnx

def compile_for_snapdragon():
    print("Exporting YOLOv8n to ONNX locally...")
    yolo_model = YOLO('yolov8n.pt')
    onnx_path = yolo_model.export(format='onnx', imgsz=640)
    
    print("Sanitizing ONNX graph for Qualcomm AI Hub strict parser...")
    # Fix 'output0' appearing in both value_info and output
    onnx_model = onnx.load(onnx_path)
    to_remove = [vi for vi in onnx_model.graph.value_info if vi.name == 'output0']
    for vi in to_remove:
        onnx_model.graph.value_info.remove(vi)
    
    fixed_onnx_path = onnx_path.replace('.onnx', '_fixed.onnx')
    onnx.save(onnx_model, fixed_onnx_path)

    # Define Target Device for the Hackathon
    target_device = hub.Device("Snapdragon X Elite CRD")
    print(f"Submitting compile job to Qualcomm AI Hub for {target_device.name}...")

    compile_job = hub.submit_compile_job(
        model=fixed_onnx_path,
        device=target_device,
    )
    
    print(f"Compile Job Status: {compile_job.get_status()}")
    target_model = compile_job.get_target_model()
    
    print("Submitting hardware profiling job to generate hackathon metrics...")
    # Profile the compiled model on real cloud-hosted Snapdragon hardware
    profile_job = hub.submit_profile_job(
        model=target_model,
        device=target_device,
    )
    
    print("Downloading target NPU model...")
    # This downloads the compiled .onnx/.qnn model for local execution
    target_model.download("yolov8n_snapdragon_npu.onnx")
    
    print(f"Profiling complete. Latency/Memory metrics: {profile_job.download_profile()}")

if __name__ == "__main__":
    compile_for_snapdragon()
