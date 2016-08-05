#pragma once
#include "useOpenCV2410.h"

#define STYLE_HISTO_COLOR		"Color Histogram"
#define STYLE_HISTO_GRADIENT	"Histogram of Gradient"
#define STYLE_HISTO_LUMINANCE	"Luminance Histogram"
#define STYLE_HOG_PCA			"PCA of HOG"
#define STYLE_MPEG7_EHD			"MPEG7_Edge Histogram Descriptor"
#define STYLE_MPEG7_CLD			"MPEG7_Color Layout Descriptor"
#define STYLE_MPEG7_DCD			"MPEG7_Dominant Color Descriptor"
#define STYLE_MPEG7_CSD			"MPEG7_Color Structure Descriptor"
#define STYLE_MPEG7_HTD			"MPEG7_Homogeneous Texture Descriptor"
#define STYLE_MPEG7_SCD			"MPEG7_Scalable Color Descriptor"
#define STYLE_MPEG7_CSD			"MPEG7_Color Structure Descriptor"

class abstractExtract
{
protected:

	int m_resize_width;
	int m_resize_height;
	int m_ori_width;
	int m_ori_height;
	Mat* m_img;
	int m_nBins;
	const char* m_feautre_name;
	vector< vector < float> > m_descriptorsValues;   
	vector< vector < Point> > m_locations;   
	virtual void setFeatureName(const char* name)=0;
public:
	abstractExtract() : m_img (NULL), m_resize_height(0), m_resize_width(0), m_ori_width(0), m_ori_height(0), m_nBins(0) {

	}
	~abstractExtract() {

	}
	virtual void saveXML(char* SaveHogDesFileName) = 0;
	virtual void setImageSizes(int ori_width, int ori_height, int resize_width=450, int resize_height=360) =0;
	virtual void setResizeInfo(int width=450, int height=360) =0;
	virtual void ref_Image(Mat* img) =0;
	virtual void init_image() = 0;
	virtual void showImage(char* windowName) = 0;
	virtual void extract() = 0;
	
};
