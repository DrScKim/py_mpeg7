// StyleSimillar.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "base_Extract.h"
#include "FileHandler.h"
#include "feature_schema.h"
#include "indexer.h"
#include "dim_reduction.h"
#include "search.h"


void trainIndex();
void loadIndex(char* dbPat);
int _tmain(int argc, _TCHAR* argv[])
{
	std::locale::global( std::locale( "kor" ) ); //"UTF-8") );
	std::cout << "Set locale to " << std::locale().name() << std::endl;

	//trainIndex();
	loadIndex("C:/infoTrainDB.idx");

	return 0;
}

void loadIndex(char* dbPath)
{
	MMIndexer mind;
	char path[256];
	mind.loadIndex(dbPath);
	int flag= COLOR_ONLY;
	printf("input path of test image : ");
	scanf("%s",path);
	while(strcmp(path,"quit")!=0) {
		if(strcmp(path, "color")==0) flag=COLOR_ONLY;
		else if(strcmp(path, "hog")==0) flag=HOG_ONLY;
		else if(strcmp(path, "both")==0) flag=HOG_COLOR;
		else if(strcmp(path, "cld")==0) flag=MPEG7_CLD;
		else if(strcmp(path, "ehd_l1")==0) flag=MPEG7_EHD_L1;
		else if(strcmp(path, "ehd")==0) flag=MPEG7_EHD;
		else if(strcmp(path, "htd")==0) flag=MPEG7_HTD;
		else {
			printf("%s\n", path);
			Mat img = imread(path);
			if(img.rows != 0)  {
				namedWindow("original",CV_WINDOW_AUTOSIZE);
				imshow("original", img);
				mind.NNSearch(img, 6,flag, true);
				cv::waitKey(0);
				destroyAllWindows();

			} else {
				printf("File Not FOUND!\n");
			}
			img.release();
		}

		//printf("Change Option input \"color\" : color Only,  \"hog\" : hog Only, \"both\" : both feature ");
		printf("input path of test image : ");
		scanf("%s",path);
	}
};

void trainIndex()
{
	MMIndexer mind;
	char path[256];

	int flag= HOG_COLOR;

	
	//mind.indexStyleLeraning("C:/styleIndex.idx",L"E:\\StyleSimDB\\all-Dataset\\Clip Art",450,360,450,360);
	mind.indexStyleLeraning("C:/infoTrainDB.idx",INFO_DB_PATH,450,360,450,360);
	
	printf("input path of test image : ");
	scanf("%s",path);
	while(strcmp(path,"quit")!=0) {
		if(strcmp(path, "color")==0) flag=COLOR_ONLY;
		else if(strcmp(path, "hog")==0) flag=HOG_ONLY;
		else if(strcmp(path, "both")==0) flag=HOG_COLOR;
		else if(strcmp(path, "cld")==0) flag=MPEG7_CLD;
		else if(strcmp(path, "ehd_l1")==0) flag=MPEG7_EHD_L1;
		else if(strcmp(path, "ehd")==0) flag=MPEG7_EHD;
		else if(strcmp(path, "htd")==0) flag=MPEG7_HTD;
		else {
			printf("%s\n", path);
			Mat img = imread(path);
			if(img.rows != 0)  {
				namedWindow("original",CV_WINDOW_AUTOSIZE);
				imshow("original", img);
				mind.NNSearch(img, 6,flag, true);
				cv::waitKey(0);
				destroyAllWindows();

			} else {
				printf("File Not FOUND!\n");
			}
			img.release();
		}

		//printf("Change Option input \"color\" : color Only,  \"hog\" : hog Only, \"both\" : both feature ");
		printf("input path of test image : ");
		scanf("%s",path);

	}
	cout << "Index complete!" << endl;
	
	
	
	
	

	/*
	int nFeatures = mind.getNFeatures();
	baseExtract* HoG = (HoGExtractor*)mind.getExtractedFeature(0, STYLE_HISTO_GRADIENT);
	int nDim = HoG->getDim();
	dim_reduction dim_reduct(nDim, nFeatures);

	cout << "COL : " << nFeatures << " ROW: " << nDim << endl;
	for(int i=0; i<nFeatures; i++) {
		baseExtract* HE = (HoGExtractor*)mind.getExtractedFeature(i, STYLE_HISTO_GRADIENT);
		vector<float> d;
		HE->get1DDescriptor(d);
		dim_reduct.push_feature(d);
	}
	cout << "PCA Start!" << endl;
	dim_reduct.perform(230);
	cout << "PCA Finished!" << endl;
	system("pause");
	for(int i=0; i<nFeatures; i++) {
		baseExtract* HE = (HoGExtractor*)mind.getExtractedFeature(i, STYLE_HISTO_GRADIENT);
		//dim_reduct.push_feature(HE->get1DDescriptor());
		dim_reduct.project(230);
	}
	*/
}