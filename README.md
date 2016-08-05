# py_mpeg7
----------------------------------------------------------------------------------------

this is wrapping function to use mpeg7 feature in python and calculate its distance

the library is adpated from the MPEG-7 XM Reference Software available at 
http://standards.iso.org/ittf/PubliclyAvailableStandards/index.html



----------------------------------------------------------------------------------------

1. Setting

	Additional include directory : {OPENCV_INCLUDE_PATH};{BOOST_LIBRARY_INCLUDE_PATH};{PYTHON27_INCLUDE_PATH}
	Additional library directory : {OPENCV_LIB_PATH};{BOOST_LIB_INCLUDE_PATH};{PYTHON27_LIB_PATH}
----------------------------------------------------------------------------------------
2. description
	Currently Supported Features

	+ Color Structure Descriptor (CSD)
	+ Color Layout Descriptor (CLD)
	+ Dominant Color Descriptor (DCD)
	+ Homogeneous Texture Descriptor (HTD)
	+ Edge Histogram Descriptor (EHD)
	

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
----------------------------------------------------------------------------------------
3. Dependencies

	+ boost 1.61 : http://www.boost.org/users/history/version_1_61_0.html
	+ python 2.7 : https://www.python.org/download/releases/2.7/
	+ opencv 2.4.10 : http://opencv.org/downloads.html
----------------------------------------------------------------------------------------
4. How to use
	
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

----------------------------------------------------------------------------------------

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
