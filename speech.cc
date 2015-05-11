#include "speech-util/speech.h"
#include <fstream>
#include "ebt/ebt.h"

namespace speech {

    std::unordered_set<std::string> load_phone_set(std::string filename)
    {
        std::unordered_set<std::string> result;
    
        std::ifstream ifs{filename};
    
        std::string line;
        while (std::getline(ifs, line)) {
            result.insert(line);
        }
    
        return result;
    }

    std::vector<double>
    clarkson_moreno_feature(
        std::vector<std::vector<double>> utt, int start, int end, int nfeat)
    {
        std::vector<double> result;
    
        end = std::min<int>(utt.size(), end);
    
        int size = std::max<int>(0, end - start);
    
        int dim = std::min<int>(nfeat, utt[0].size());
    
        if (size <= 0) {
            result.resize(dim * 5 + 21);
            result.push_back(1);
            return result;
        }
    
        std::vector<double> vec;
        vec.resize(dim);
    
        for (int i = start; i < size * 0.3 + start; ++i) {
            for (int j = 0; j < dim; ++j) {
                vec[j] += utt[i][j] / int(size * 0.3 + 1);
            }
        }
    
        for (int i = 0; i < vec.size(); ++i) {
            result.push_back(vec[i]);
        }
        vec.clear();
        vec.resize(dim);
    
        for (int i = size * 0.3 + start; i < size * 0.7 + start; ++i) {
            for (int j = 0; j < dim; ++j) {
                vec[j] += utt[i][j] / int(size * 0.4 + 1);
            }
        }
    
        for (int i = 0; i < vec.size(); ++i) {
            result.push_back(vec[i]);
        }
        vec.clear();
        vec.resize(dim);
    
        for (int i = size * 0.7 + start; i < size + start; ++i) {
            for (int j = 0; j < dim; ++j) {
                vec[j] += utt[i][j] / int(size * 0.3 + 1);
            }
        }
    
        for (int i = 0; i < vec.size(); ++i) {
            result.push_back(vec[i]);
        }
        vec.clear();
        vec.resize(dim);
    
        for (int i = std::max<int>(start - 2, 0); i < std::min<int>(start + 2, utt.size()); ++i) {
            for (int j = 0; j < dim; ++j) {
                vec[j] += utt[i][j] / 4;
            }
        }
    
        for (int i = 0; i < vec.size(); ++i) {
            result.push_back(vec[i]);
        }
        vec.clear();
        vec.resize(dim);
    
        for (int i = std::max<int>(end - 2, 0); i < std::min<int>(end + 2, utt.size()); ++i) {
            for (int j = 0; j < dim; ++j) {
                vec[j] += utt[i][j] / 4;
            }
        }
    
        for (int i = 0; i < vec.size(); ++i) {
            result.push_back(vec[i]);
        }
        vec.clear();
        vec.resize(dim);
    
        for (int i = 0; i <= 20; ++i) {
            result.push_back((i == size) ? 1 : 0);
        }
        result.push_back(1);
    
        return result;
    }

    std::vector<std::vector<double>> load_frames(std::string filename, int nfeat)
    {
        std::vector<std::vector<double>> result;
        std::ifstream ifs { filename };
    
        std::string line;
        while (std::getline(ifs, line)) {
            std::vector<double> vec;
    
            std::vector<std::string> parts = ebt::split(line);
            for (auto& p: parts) {
                if (vec.size() == nfeat) {
                    break;
                }
                vec.push_back(std::stod(p));
            }
    
            result.push_back(vec);
        }
    
        return result;
    }
    
}
