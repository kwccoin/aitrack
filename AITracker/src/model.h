#pragma once

#include "opencv.hpp"
#include <onnxruntime_cxx_api.h>
#include "data.h"
#include "imageprocessor.h"
#include "PositionSolver.h"
#include "filters.h"

class Tracker
{

public:
	PositionSolver* solver;

	Tracker(PositionSolver* solver, std::wstring& detection_model_path, std::wstring& landmark_model_path);
	~Tracker();
	void predict(cv::Mat& image, FaceData& face_data, IFilter *filter=nullptr);

private:
	ImageProcessor improc;

	std::string detection_model_path;
	Ort::Env* enviro;
	Ort::SessionOptions* session_options;
	Ort::Session* session;
	Ort::Session* session_lm;
	Ort::AllocatorWithDefaultOptions* allocator;
	Ort::MemoryInfo* memory_info;

	std::vector<const char*> detection_input_node_names;
	std::vector<const char*> detection_output_node_names;
	std::vector<const char*> landmarks_input_node_names;
	std::vector<const char*> landmarks_output_node_names;


	size_t tensor_input_size;
	int64_t tensor_input_dims[4] = { 1,3,224,224 };
	int tensor_detection_output_dims[4] = { 1,2,56,56 };

	// Buffer for transposing the input of the neural networks
	float buffer_data[150528];

	void detect_face(const cv::Mat& image, FaceData& face_data);
	void proc_face_detect(float* face, float width = 1080, float height = 720);
	void detect_landmarks(const cv::Mat& image, int x0, int y0, float scale_x, float scale_y, FaceData& face_data);
	void proc_heatmaps(float* heatmaps, int x0, int y0, float scale_x, float scale_y, FaceData& face_data);
};
