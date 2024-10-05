# onnx 转化 ncnn 文件操作流程 (yolov5-lite)

## 生成 onnx 文件
`python3 export.py --weights best.pt --img-size 320`

## 简化 onnx 模型
`onnxsim best.onnx best-sim.onnx`

## 使用 onnx2ncnn 转化 onnx 模型
`./tools/onnx/onnx2ncnn best-sim.onnx best.param best.bin`

## 使用 ncnnoptimize 对 ncnn 模型进行优化
`./tools/ncnnoptimize best.param best.bin best-opt.param best-opt.bin 65536`

## 修改 best-opt.param reshape 参数  
` reshape 0 = -1` 以支持视频输入
