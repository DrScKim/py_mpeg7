#pragma once
#include "abstract_Extract.h"
#include <iostream>
#include <string>
using namespace std;

#include <boost/python.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/foreach.hpp> 

using namespace boost::python;

#define JSON(str, key, val)	 str.append("\""); str.append(key); str.append("\" : \""); str.append(val);  str.append("\"");
#define JSONARRAY(str, key, val)	 str.append("{\""); str.append(key); str.append("\" : \""); str.append(val);  str.append("\"}");

class baseExtract : public abstractExtract
{
protected:

	int m_resize_width;
	int m_resize_height;
	int m_ori_width;
	int m_ori_height;
	Mat* m_img;
	
	int m_nBins;
	vector< vector < float> > m_descriptorsValues;   
	vector< vector < Point> > m_locations;   
	
	void setOriSizeAfterInit() {
		m_ori_width = m_img->rows;
		m_ori_height = m_img->cols;
	}
	void get_float_values(string str, vector<float> &r)
	{
		int pos=0;
		string token;
		int i=0;
		while((pos = str.rfind(',')) != string::npos) {
			token = str.substr(pos+1, str.length());
			float f = stof(token);

			r.push_back(f);
			str.erase(pos, str.length());
			i++;
		}
		float f = stof(str);
		r.push_back(f);
	}
	
public:
	baseExtract() : m_img (NULL), m_resize_height(0), m_resize_width(0), m_ori_width(0), m_ori_height(0), m_nBins(0) {
		setFeatureName("base");
	}
	baseExtract(int dim) : m_img (NULL), m_resize_height(0), m_resize_width(0), m_ori_width(0), m_ori_height(0), m_nBins(0) {
		setDim(dim);
	}
	baseExtract(int resize_hight, int resize_width, int ori_width, int ori_height, int nBins) {

	}
	virtual ~baseExtract() {
		if(!m_img) delete m_img;
		m_descriptorsValues.clear();
		m_locations.clear();
		//std::cout << "Base Extract Clear!" << std::endl;
		
	}
	bool typeCompare(const char* typeName) {
		//cout << this->getName() << ", " << typeName << endl;
 		return (strcmp(this->getName(), typeName) == 0);
	}
	void setFeatureName(const char* name) {
		m_feautre_name=name;
	}
	void loadXML() {

	}
	void saveXML(char* SaveDesFileName) {
		FileStorage hogXml(SaveDesFileName, FileStorage::WRITE); //FileStorage::READ   
		//2d vector to Mat   
		int row=m_descriptorsValues.size(), col=m_descriptorsValues[0].size();   
		printf("col=%d, row=%d\n", row, col);   
		Mat M(row,col,CV_32F);   
		//save Mat to XML   
		for(int i=0; i< row; ++i)     
			memcpy( &(M.data[col * i * sizeof(float) ]) ,m_descriptorsValues[i].data(),col*sizeof(float));   
		//write xml   
		write(hogXml, "Descriptor_of_images",  M);   
		//write(hogXml, "Descriptor", v_descriptorsValues );   
		//write(hogXml, "locations", v_locations );   
		hogXml.release();  
	}
	void setImageSizes(int ori_width, int ori_height, int resize_width=450, int resize_height=360) {
		m_ori_width= ori_width; 
		m_ori_height=ori_height;
		m_resize_width = resize_width;
		m_resize_height = resize_height;
	}
	void setResizeInfo(int width=450, int height=360) {
		m_resize_width = width; m_resize_height = height;
	}
	void ref_Image(Mat* img) {
		m_img = img;
	}
	virtual void init_image() {}
	virtual void showImage(char* windowName) {}
	virtual void extract() {}

	string getDescriptor(int i)
	{
		string str ="";
		for(int j=0; j<m_descriptorsValues[i].size()-1; j++){
			str.append(to_string((long double)m_descriptorsValues[i][j]));
			str.append(",");
		}
		str.append(to_string((long double)m_descriptorsValues[i][m_descriptorsValues[i].size()-1]));
		return str;

	}
	void deserialize(const char* features) {
		string str1(features);

		std::stringstream  ss;
		ss << str1;
		float val=0.f;

		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);
		vector<float> vals;
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("descriptor.features")) 
		{ 
			get_float_values(v.second.get<std::string>("feature"), vals);
			m_descriptorsValues.push_back(vals);
			//this->setBins(vals);
			vals.clear();
		} 
	}
	string serialize() 
	{
		string str = "{";
		str.append("\"descriptor\" : {");
		JSON(str, "feature_name", getName());str.append(",");
		str.append("\"features\" : [");
		for(int i=0; i<m_descriptorsValues.size(); i++) {
			JSONARRAY(str,"feature", getDescriptor(i));
			//str.append("}");
			if(i!=m_descriptorsValues.size()-1)
				str.append(",");
		}
		str.append("]");
		str.append("}");
		str.append("}");

		return str;
	}

	vector<float> get1DDescriptor() {
		vector<float> d;
		for(int i=0 ; i<m_descriptorsValues.size(); i++) {
			for(int j=0; j< m_descriptorsValues[i].size(); j++) {
				d.push_back(m_descriptorsValues[i][j]);
			}
		}
		return d;
	}
	void get1DDescriptor(vector<float>& d) {
		
		for(int i=0 ; i<m_descriptorsValues.size(); i++) {
			for(int j=0; j< m_descriptorsValues[i].size(); j++) {
				d.push_back(m_descriptorsValues[i][j]);
			}
		}
	}
	vector<vector<float>> *getDscr() {
		return &m_descriptorsValues;
	}
	vector<vector<Point>>* getLocations() {
		return &m_locations;
	}
	void setDim(int n) {m_nBins = n;}
	void setBins(vector<float>& feats) {
		m_descriptorsValues.push_back(feats);
	}
	int getDim() {return m_descriptorsValues.size() * m_descriptorsValues[0].size();}
	int getRow() {return m_descriptorsValues[0].size();}
	int getCol() {return m_descriptorsValues.size();}
	const char* getName() {return m_feautre_name;}
	float getBin(int r, int c) { return 0.f;}

	float L1Dist(baseExtract& dscr) {
		float dist=0;
		if( this->m_descriptorsValues.size() == dscr.getDscr()->size()) {
			for(int x=0; x<m_descriptorsValues.size(); x++) 
			{
				for(int y =0; y < m_descriptorsValues[0].size(); y++) {
					dist += std::abs(m_descriptorsValues.at(x)[y]-dscr.getDscr()->at(x)[y]);
				}
			}
		} else {
			//TODO: calculate for other size
		}
		return dist/(this->m_descriptorsValues.size()*m_descriptorsValues[0].size());
	}
	virtual float dist(baseExtract* feat)  {
		return 0.f;
	}
	float operator-(baseExtract& dscr) //L1 dist
	{
		float dist=0;
		if( this->m_descriptorsValues.size() == dscr.getDscr()->size()) {
			for(int x=0; x< m_descriptorsValues.size(); x++) 
			{
				for(int y =0; y < m_descriptorsValues[0].size(); y++) {
					dist += std::abs(m_descriptorsValues.at(x)[y]-dscr.getDscr()->at(x)[y]);
				}
			}
		} else {
			//TODO: calculate for other size
		}
		return dist/(this->m_descriptorsValues.size()*m_descriptorsValues[0].size());
	}
};
