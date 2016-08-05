// boost-python-test.cpp : Defines the exported functions for the DLL application.
//

/*

this is wrapping function to use mpeg7 feature in python and calculate its distance

the library is adpated from the MPEG-7 XM Reference Software available at 
http://standards.iso.org/ittf/PubliclyAvailableStandards/index.html

As also available in the header files adapted from the XM software, please note that:
"ISO/IEC gives users of MPEG-7 free license to this software module or
modifications thereof for use in hardware or software products claiming
conformance to MPEG-7".

The XM reference software contains some bugs and does not work properly.
Moreover, it is not meant to be used in applications directly, therefore, 
there is no decent interface to use it easily, not mentioning the horrible coding, 
which makes everybody run away at the first glance!
This library is an attempt to make the MPEG-7 feature extraction easier to integrate into new systems..

----------------------------------------------------------------------------------------

Supported descriptors in this library :

+ Color Structure Descriptor (CSD)
+ Scalable Color Descriptor (SCD)
+ GoF/GoP Color Descriptor (GoF/GoP)
+ Color Layout Descriptor (CLD)
+ Dominant Color Descriptor (DCD)
+ Homogeneous Texture Descriptor (HTD)
+ Edge Histogram Descriptor (EHD)
+ Face Recognition Descriptor (FRD)

Please see http://www.chiariglione.org/mpeg/standards/mpeg-7/mpeg-7.htm for a brief description of each.
For more details: 
B. S. Manjunath, P. Salembier, and T. Sikora, Eds., Introduction to MPEG-7: Multimedia Content Description Interface. England: WILEY, 2002.

-----------------------------------------------------------------------------------------


1. Setting

	Additional include directory : {OPENCV_INCLUDE_PATH};{BOOST_LIBRARY_INCLUDE_PATH};{PYTHON27_INCLUDE_PATH}
	Additional library directory : {OPENCV_LIB_PATH};{BOOST_LIB_INCLUDE_PATH};{PYTHON27_LIB_PATH}

2. description
	
	MPEG7 features which is except for Dominant Color Descriptor calculate distance as its standard measure
	Dominant Color Descriptors distance measure is tunned by me
	the measure is Earth Mover's Distance
	because this measure improve search result according to 

	get_serialized_feature(const char* img_path, const char* type) 
	the function extract MPEG7 feature
	and return json type of feature as below

	CLD(Color Layout Descriptor) Example
	{
		"descriptor": {
			"feature_name": "MPEG7_Color Layout Descriptor",
			"features": [{
				"feature": "48,16,13,8,16,16,16,16,16,15,9,16,16,16,16,16,16,16,16,16,16,13,16,16,16,16,16,16,16,16,16,16,16,16,16,17,16,16,16,16,16,16,16,16,16,16,16,16,16,17,16,16,16,16,16,16,16,16,16,16,16,16,16,16"
			}, {
				"feature": "25,16,17,14,16,16,16,16,16,17,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16"
			}, {
				"feature": "49,16,21,27,16,16,16,16,16,18,23,16,16,16,16,16,16,16,16,16,16,19,16,16,16,16,16,16"
			}]
		}
	}

	DCD(Dominant Color Descripto) Example
	features 
	first one is DC, DCD has 1~8 DC(Dominant Color)  
	second one is R,G,Bs values of each DC
	last one it's R,G,Bs variance of each DC
	{
		"descriptor": {
			"feature_name": "MPEG7_Dominant Color Descriptor",
			"features": [{
				"feature": "57,3,7,5,13,6,5"
			}, {
				"feature": "12,2,2,195,186,181,119,62,29,175,120,44,63,13,5,239,190,100,66,47,38"
			}, {
				"feature": "9,20,3,240,31,157,42,80,43,37,70,54,32,55,20,27,82,55,99,45,52"
			}]
		}
	}

3. How to use
	
	after release copy of file "cv_mpeg7.pyd" located on $(SolutionDir)$(Configuration)\
	and paste your python project folder and import {your_app.cv_mpeg7}

	
	feature extract example is below code
	######################################################################################
	######################################################################################

	import myapp.cv_mpeg7
	my_file_path = "C:/abc.jpg" #your image file path
	type_dcd = myapp.cv_mpeg7.get_mpeg7_dcd_name()
	feat1 = myapp.cv_mpeg7.get_serialized_feature(my_file_path, type_dcd)
	feat2 = myapp.cv_mpeg7.get_serialized_feature(my_file_path, type_dcd)
	dist = myapp.cv_mpeg7.get_distance(feat1, feat2, type_dcd)

	######################################################################################
	######################################################################################

	if you have any question of this project
	feel free to contact me through my e-mail

	Copyright (C) 1998 Sangchul Kim
	Computer Science Department, Sogang University
	email : smaslayer1@gmail.com
	hompy : http://mmlab.sogang.ac.kr


	date.2016.08.04
	//TODO list:
	adapt these feature that blows
	+ Scalable Color Descriptor (SCD)
	+ Face Recognition Descriptor (FRD)
	+ GoF/GoP Color Descriptor (GoF/GoP)

*/


#include <iostream>
#include <boost/python.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/foreach.hpp> 

#include "base_Extract.h"
#include <opencv2/core/core.hpp>

#include "util.h"
#include "MPEG7_Extract.h"
#include "Color_Extract.h"

#define IS_SAME_TYPE(in_type, out_type)	(strcmp(in_type, out_type)==0)

using namespace std;
using namespace cv;

using namespace boost::python;
//object main_module = import("__main__");

__declspec(dllimport) string get_serialized_feature (const char* img_path, const char* type);
__declspec(dllimport) float get_distance ( const char* feat1, const char* feat2, const char* feat_type);
__declspec(dllimport) const char* get_mpeg7_ehd_name();
__declspec(dllimport) const char* get_mpeg7_dcd_name();
__declspec(dllimport) const char* get_mpeg7_cld_name();
__declspec(dllimport) const char* get_mpeg7_htd_name();
__declspec(dllimport) const char* get_mpeg7_ehd_name();
__declspec(dllimport) const char* get_mpeg7_csd_name();
__declspec(dllimport) const char* get_mpeg7_scd_name();


const char* get_mpeg7_ehd_name()
{
	return STYLE_MPEG7_EHD;
}
const char* get_mpeg7_dcd_name()
{
	return STYLE_MPEG7_DCD;
}
const char* get_mpeg7_htd_name()
{
	return STYLE_MPEG7_HTD;
}
const char* get_mpeg7_cld_name()
{
	return STYLE_MPEG7_CLD;
}
const char* get_mpeg7_scd_name()
{
	return STYLE_MPEG7_SCD;
}
const char* get_mpeg7_csd_name()
{
	return STYLE_MPEG7_CSD;
}

float get_distance( const char* feat1, const char* feat2, const char* feat_type )
{
	baseExtract* base_feat1 = new baseExtract();
	baseExtract* base_feat2 = new baseExtract();
	base_feat1->deserialize(feat1);
	base_feat2->deserialize(feat2);
	
	float val=65535.f;
	
	if(IS_SAME_TYPE(feat_type, STYLE_MPEG7_CLD)) {
		CLDExtract* cld = new CLDExtract();
		CLDExtract* cld2 = new CLDExtract();
		cld->generate(base_feat1);
		cld2->generate(base_feat2);
		val = cld->dist((baseExtract*) cld2);
		delete cld; delete cld2;
	}
	if(IS_SAME_TYPE(feat_type, STYLE_HISTO_COLOR)) {
		val = base_feat1->L1Dist( *base_feat2);
	}
	if(IS_SAME_TYPE(feat_type, STYLE_MPEG7_DCD)) {
		DCDExtract* dcd = new DCDExtract();
		DCDExtract* dcd2 = new DCDExtract();
		dcd->generate(base_feat1);
		dcd2->generate(base_feat2);
		val = dcd->dist((baseExtract*) dcd2);
		delete dcd; delete dcd2;
	}
	if(IS_SAME_TYPE(feat_type, STYLE_MPEG7_EHD)) {
		EHDExtract* ehd = new EHDExtract();
		EHDExtract* ehd2 = new EHDExtract();
		ehd->generate(base_feat1);
		ehd2->generate(base_feat2);
		val = ehd->dist((baseExtract*)ehd2);
		delete ehd; delete ehd2;
	}
	if(IS_SAME_TYPE(feat_type, STYLE_MPEG7_HTD)) {
		homogeneousTextureExtract* htd = new homogeneousTextureExtract();
		htd->generate(base_feat1);
		homogeneousTextureExtract* htd2 = new homogeneousTextureExtract();
		htd2->generate(base_feat2);
		val = htd->dist((baseExtract*)htd2);
		delete htd; delete htd2;
	}
	delete base_feat1;
	delete base_feat2;
	return val;
}


string get_serialized_feature(const char* img_path, const char* type)
{
	
	Mat img=imread(img_path);
	string returnVal;
	if(IS_SAME_TYPE(type, get_mpeg7_dcd_name())) {

		DCDExtract* dcd = new DCDExtract();
		dcd->ref_Image(&img);
		dcd->extract();
		printf("extract complete!\n");
		img.release();

		returnVal = dcd->serialize();
		delete dcd;
		return returnVal;
	}
	if(IS_SAME_TYPE(type, get_mpeg7_csd_name())) {
		CSDExtract* csd = new CSDExtract();
		csd->ref_Image(&img);

		csd->extract();
		img.release();
		returnVal = csd->serialize();
		delete csd;
		return returnVal;
	}
	if(IS_SAME_TYPE(type, get_mpeg7_cld_name())) {
		CLDExtract* cld = new CLDExtract();
		cld->ref_Image(&img);
		cld->extract();
		img.release();
		returnVal = cld->serialize();
		delete cld;
		return returnVal;
	}
	else if(IS_SAME_TYPE(type, get_mpeg7_ehd_name())) {
		EHDExtract* ehd = new EHDExtract();
		ehd->ref_Image(&img);
		ehd->extract();
		img.release();
		returnVal= ehd->serialize();
		delete ehd;
		return returnVal;
	}
	else if(IS_SAME_TYPE(type, get_mpeg7_htd_name())) {
		homogeneousTextureExtract* htd = new homogeneousTextureExtract();
		htd->ref_Image(&img);
		htd->extract();
		img.release();
		returnVal= htd->serialize();
		delete htd;
		return returnVal;
	}
	
	return string("{\"descriptor\":\"none\"}");
}


BOOST_PYTHON_MODULE(cv_mpeg7)
{
	
	def("get_mpeg7_ehd_name", get_mpeg7_ehd_name);
	def("get_mpeg7_dcd_name", get_mpeg7_dcd_name);
	def("get_mpeg7_cld_name", get_mpeg7_cld_name);
	def("get_mpeg7_htd_name", get_mpeg7_htd_name);
	def("get_mpeg7_ehd_name", get_mpeg7_ehd_name);
	def("get_mpeg7_csd_name", get_mpeg7_csd_name);
	def("get_mpeg7_scd_name", get_mpeg7_scd_name);
	def("get_distance", get_distance);
	def("get_serialized_feature", get_serialized_feature);
	//def("");
}