#include <map>
#include <fstream>
#include <string>
#include <optional>
#include <math.h>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <eigen3/Eigen/Dense>

#include "csv_iterator.hpp"

// timestamp
// p_RS_R_x [m], p_RS_R_y [m], p_RS_R_z [m]
// q_RS_w [], q_RS_x [], q_RS_y [], q_RS_z []
// v_RS_R_x [m s^-1], v_RS_R_y [m s^-1], v_RS_R_z [m s^-1]
// b_w_RS_S_x [rad s^-1], b_w_RS_S_y [rad s^-1], b_w_RS_S_z [rad s^-1]
// b_a_RS_S_x [m s^-2], b_a_RS_S_y [m s^-2], b_a_RS_S_z [m s^-2]

using namespace ILLIXR;

typedef pose_type sensor_types;

static
std::map<ullong, sensor_types>
load_data(const std::string& data_path) {
	std::map<ullong, sensor_types> data;

	std::ifstream ground_truth_file {data_path + "state_groundtruth_estimate0/data.csv"};
	if (!ground_truth_file.good()) {
		std::cerr << (data_path + "state_groundtruth_estimate0/data.csv") << " is not good" << std::endl;
		abort();
	}
	for(CSVIterator row{ground_truth_file, 1}; row != CSVIterator{}; ++row) {
		ullong t = floor(std::stoull(row[0]) / 10000);
		Eigen::Vector3f av {std::stof(row[1]), std::stof(row[2]), std::stof(row[3])};
		Eigen::Quaternionf la {std::stof(row[4]), std::stof(row[5]), std::stof(row[6]), std::stof(row[7])};
		data[t] = {{}, av, la};
	}

	return data;
}
