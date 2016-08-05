#pragma once
#include "ExtractHeader.h"
#include <fstream>

#define JSON(str, key, val)	str.append("\"");  str.append(key); str.append("\" : \""); str.append(val);  str.append("\"");

class feature_schema
{
private:
	int m_nTypeOfFeature;
	int m_nBins;	//read only
	int m_nCells;	//read only
	vector<baseExtract> m_features;
	string filename;
	//string feat_name;
	
public:
	feature_schema() :filename(), m_nTypeOfFeature(0), m_nBins(0), m_nCells(0){
	}
	~feature_schema() {
		m_features.clear();
	}
	void addFeature(baseExtract& feat) {
		m_features.push_back(feat);
		m_nTypeOfFeature++;
	}
	void addFeature(baseExtract* feat) {
		addFeature(*feat);
	}
	void clearFeature() {
		m_features.clear();
	}
	int getFeatureSize() {
		return m_features.size();
	}
	baseExtract* getFeaturePTR(int idx) {
		return &(m_features[idx]);
	}
	const char* getFeatureName(int i) {
		return m_features[i].getName();
	}
	const char* getFileName() {
		return filename.c_str();
	}
	void setFileName(string str) {
		filename = str;
	}
	/*
	string serialize() {
		string str;
		str << "{ "
		JSON(str,"typeOfFeature", std::to_string(m_nTypeOfFeature) m_nTypeOfFeature), str.append(",	");
		str << "\"typeOfFeature\" : " << string(m_nTypeOfFeature) << "\", ";
		str << "\"typeOfFeature\" : " << string(m_nTypeOfFeature) << "\", ";
		str << "\"typeOfFeature\" : " << string(m_nTypeOfFeature) << "\", ";
		}
	*/
	void write(std::ofstream& fs, wchar_t* filename) {
		fs << filename;
		for(int i=0; i< m_features.size(); i++) {
			baseExtract feat = m_features[i];
			fs << feat.getName() << endl;		//the name of feature
			fs << feat.getDim();				//Dimension
			fs << feat.getDscr()->size();		//Size of Bins
			fs << feat.getDscr()->at(0).size();	//Size of Cells

			for(int cell = 0; cell < feat.getDscr()->at(0).size(); cell++) {
				for(int bin = 0; bin< feat.getDscr()->size(); bin++) {
					fs << feat.getDscr()->at(cell)[bin]<< '\t';
				}
				fs << '\n';
			}
			fs << '\n';
		}
	}
	void write(std::ofstream& fs, char* filename) {
		fs << filename << endl;
		fs << m_features.size() << endl;
		for(int i=0; i< m_features.size(); i++) {
			baseExtract feat = m_features[i];
			fs << feat.getName() << endl;		//the name of feature
			//fs << feat.getDim() << endl;				//Dimension
			
			fs << feat.getDscr()->size()<<endl;		//Size of Cells
			
			std::cout.precision(9);
			//cout << "Cells : " << feat.getDscr()->at(0).size()<< endl;
			//cout << "Bins : " << feat.getDscr()->size()<< endl;

			for(int cell = 0; cell < feat.getDscr()->size(); cell++) {
				fs << feat.getDscr()->at(cell).size() << endl;	//Size of Bins
				//cout << "CELL: " << cell << endl;
				for(int bin = 0; bin< feat.getDscr()->at(cell).size(); bin++) {
					//cout << bin <<" : " << feat.getDscr()->at(cell)[bin] << endl;
					fs << feat.getDscr()->at(cell)[bin]<< '\t';
				}
				fs << '\n';
			}
		}
	}
	void read(std::ifstream& fs) {
		//fs >> filename;
		string feat;
		int nFeats=0;
		std::getline(fs, filename);
		
		std::getline(fs, feat);
		nFeats = atoi(feat.c_str());

		for(int i=0; i<nFeats; i++) {
			int nDim = 0, nRow=0, nCol=0;
			string feats;
			string feat_name;
			std::getline(fs, feat_name);
			
			fs >> nCol;
			baseExtract* feat = alloc_feature(feat_name.c_str());
			
			for(int k=0; k< nCol; k++) {
				fs >> nRow;
				vector<float> bins;
				for(int j=0; j<nRow; j++) {
					string feats;
					fs >> feats;
					bins.push_back(stof(feats));
				}
				
				feat->setBins(bins);
				
				bins.clear();
			}
			m_features.push_back(*feat);
			getline(fs, feats);
		}
	}
	baseExtract* alloc_feature(const char* feature_name) {
		if(strcmp(feature_name, STYLE_HISTO_GRADIENT) == 0)
			return new HoGExtractor();
		else if(strcmp(feature_name, STYLE_HISTO_COLOR) == 0)
			return new ColorExtract();
		else if(strcmp(feature_name, STYLE_MPEG7_EHD)==0)
			return new EHDExtract();
		else if(strcmp(feature_name, STYLE_MPEG7_HTD)==0)
			return new homogeneousTextureExtract();
		else if(strcmp(feature_name, STYLE_MPEG7_CLD)==0)
			return new CLDExtract();
		else if(strcmp(feature_name, STYLE_MPEG7_DCD)==0)
			return new DCDExtract();
		return NULL;
	}

};


