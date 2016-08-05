#pragma once

#include "feature_schema.h"
#include "FileHandler.h"

#include "dist.h"
#include "MPEG7_Extract.h"

#define HOG_ONLY	0
#define COLOR_ONLY	1
#define HOG_COLOR	2
#define MPEG7_EHD	3
#define MPEG7_EHD_L1	4
#define MPEG7_CLD	5
#define MPEG7_HTD	6

class MMIndexer
{
private:
	vector<feature_schema> m_feat_schemes;
	FileHandler m_FileHandler;
	vector<char*> feat_list;

	baseExtract* extractFeature(Mat &img, char* typeName) {
		baseExtract* tmp;
		if( strcmp(typeName,STYLE_HISTO_COLOR) == 0) {
			tmp = new CLDExtract();
			cldExtract(img, (CLDExtract*)tmp);
		}
		if(strcmp(typeName,STYLE_MPEG7_EHD) == 0) {
			tmp = new EHDExtract();
			ehdExtract(img, (EHDExtract*)tmp);
		}
		if(strcmp(typeName,STYLE_MPEG7_HTD) == 0) {
			tmp = new homogeneousTextureExtract();
			htdExtract(img, (homogeneousTextureExtract*)tmp);
		}
		if(strcmp(typeName,STYLE_HISTO_COLOR) == 0) {
			tmp = new ColorExtract();
			colorExtract(img, (ColorExtract*)tmp);
		}
		return tmp;
	}

	void cldExtract(Mat &img, CLDExtract* cld) {
		cld->ref_Image(&img);
		cld->extract();
	}
	void ehdExtract(Mat &img, EHDExtract* ehd) {
		ehd->ref_Image(&img);
		ehd->extract();
	}

	void colorExtract(Mat& img, ColorExtract* color_histo) {
		color_histo->setDim(10);
		color_histo->setResizeInfo();
		color_histo->ref_Image(&img);
		color_histo->extract();
	}
	void htdExtract(Mat &img, homogeneousTextureExtract* htd) {
		htd->ref_Image(&img);
		htd->extract();
	}
public:

	//vector<style_result> NNSearch(Mat& cmpImg, int n, bool showWindow=false)
	void NNSearch(Mat& cmpImg, int n, int type = HOG_COLOR, bool showWindow=false)
	{
		//color extract
		baseExtract* color_histo = new ColorExtract();

		color_histo->setDim(10);
		color_histo->setResizeInfo();
		color_histo->ref_Image(&cmpImg);
		color_histo->extract();


		baseExtract* color_layout = new CLDExtract();
		baseExtract* edgeHistoD = new EHDExtract();
		baseExtract* homoTextureD = new homogeneousTextureExtract();

		ehdExtract(cmpImg, (EHDExtract*)edgeHistoD);
		cldExtract(cmpImg, (CLDExtract*)color_layout);
		htdExtract(cmpImg, (homogeneousTextureExtract*)homoTextureD);

		float L1Dist_color	=	0.f;
		float L1Dist_hog	=	0.f;

		vector<style_result> results;
		/*
		for(int i=0; i<n; i++) {
			style_result r;
			r.path[0] = '\0';
			r.dist = 1024*1024;
			result.push_back(r);
		}
		*/
		int i=0;
		for(vector<feature_schema>::iterator iter = m_feat_schemes.begin(); iter != m_feat_schemes.end(); iter++) {

			baseExtract* CE = (HoGExtractor*)getExtractedFeature(i, STYLE_HISTO_COLOR);

			CLDExtract* cld = new CLDExtract();
			EHDExtract* ehd = new EHDExtract();

			homogeneousTextureExtract* htd = new homogeneousTextureExtract();
			baseExtract* ehd_bs = (EHDExtract*)getExtractedFeature(i, STYLE_MPEG7_EHD);
			baseExtract* cld_bs =  (CLDExtract*)getExtractedFeature(i, STYLE_MPEG7_CLD);
			baseExtract* htd_bs = (homogeneousTextureExtract*)getExtractedFeature(i, STYLE_MPEG7_HTD);
			float distance=0.f;
			ehd->generate(ehd_bs);
			cld->generate(cld_bs);
			htd->generate(htd_bs);

			if(type == HOG_COLOR || type == COLOR_ONLY)
				distance = dist::L1Dist(CE->get1DDescriptor(), color_histo->get1DDescriptor() );
			if(type == MPEG7_CLD)
				distance = color_layout->dist(cld);
			//if(type == MPEG7_CLD_L1) 
			if(type == MPEG7_EHD_L1)
				distance = dist::L1Dist(ehd->get1DDescriptor(), edgeHistoD->get1DDescriptor() );
			if(type == MPEG7_EHD)
				distance =  edgeHistoD->dist(ehd);
			if(type == MPEG7_HTD)
				distance = homoTextureD->dist(htd);
			style_result s;
			strcpy(s.path, iter->getFileName());
			s.dist = distance;
			results.push_back(s);
			/*
			for(int k=0; k<result.size(); k++) {
				if(result[k].dist > (L1Dist_color)) {
					style_result r;
					strcpy(r.path, iter->getFileName());
					r.dist= L1Dist_color + L1Dist_color;
					result.push_back(r);
					result.erase(result.begin()+k);
					//break;
				}
			}
			*/
			i++;
			delete cld;
			delete ehd;
			delete htd;
		}
		std::sort(results.begin(), results.end());
		string str="window";
		if(showWindow) {
			
			for(int x=0; x< n ; x++) {
				char path[256];
				sprintf(path, "window%d", x);
				namedWindow(string(path), CV_WINDOW_AUTOSIZE);
				Mat img = imread(results[x].path);
				imshow(string(path), img);
				
			}
		}

		for(int x=0; x<n; x++) {
			cout << results[x].path << ", " << results[x].dist << endl;
		}
		results.clear();
		//return result;
	}

	void SearchMultiFeatures(Mat& query, vector<char*> featureNames) {
		int nFeatureType = featureNames.size();
		int* distances = new int [ nFeatureType ];
		baseExtract** features = new baseExtract*[nFeatureType];
		int i = 0;

		for(int i=0; i != nFeatureType; i++) {
			features[i] = extractFeature(query,  featureNames[i]);//check
		}
		int idx=0;

		ColorExtract* color = new ColorExtract();
		CLDExtract* cld = new CLDExtract();
		EHDExtract* ehd = new EHDExtract();
		homogeneousTextureExtract* htd = new homogeneousTextureExtract();

		
		for(vector<feature_schema>::iterator iter = m_feat_schemes.begin(); iter != m_feat_schemes.end(); iter++) {
			baseExtract** db_feats = new baseExtract*[nFeatureType];
			Mat db_img = imread(iter->getFileName());

			for(int i=0; i != nFeatureType; i++) {
				db_feats[i] = extractFeature(db_img,  featureNames[i]);//check
				getExtractedFeature(idx, featureNames[i]);
			}

			baseExtract* CE = (HoGExtractor*)getExtractedFeature(i, STYLE_HISTO_COLOR);
			CLDExtract* cld = new CLDExtract();
			EHDExtract* ehd = new EHDExtract();
			homogeneousTextureExtract* htd = new homogeneousTextureExtract();
			baseExtract* ehd_bs = (EHDExtract*)getExtractedFeature(i, STYLE_MPEG7_EHD);
			baseExtract* cld_bs =  (CLDExtract*)getExtractedFeature(i, STYLE_MPEG7_CLD);
			baseExtract* htd_bs = (homogeneousTextureExtract*)getExtractedFeature(i, STYLE_MPEG7_HTD);

			int i = 0;
			for(int i=0; i != nFeatureType; i++) {
				db_feats[i] = extractFeature(query,  featureNames[i]);//check
			}

		}

		delete [] distances;
		for(int i=0; i<nFeatureType; i++) {
			delete features[i];
		}

		//delete [] features;

	}
	void addFeature(char* feat_name)
	{
		feat_list.push_back(feat_name);
	}
	void index(wstring root)
	{
		m_FileHandler.makeListAllFiles(root);
		vector<STDSTR> fileList =  m_FileHandler.getFilePaths();
		
		for(vector<STDSTR>::iterator iter=m_FileHandler.getFilePaths().begin(); iter != m_FileHandler.getFilePaths().end(); iter++) {
//			baseExtract* feature = m_feat_scheme.alloc_feature(*iter);	
//			feature->extract();
		}
	}
	void loadIndex(char* indexPath){
		
		ifstream infile(indexPath);
		int i=0;
		while(!infile.eof()) {
			feature_schema fs;
			fs.read(infile);
			m_feat_schemes.push_back(fs);
			cout << i++ << "file loaded"<<endl;
			
		}
		m_feat_schemes.pop_back();
		infile.close();
	}



	void indexStyleLeraning(const char* indexPath, wstring root, int resizeW, int resizeH, int WinSizeW, int WinSizeH )//extract features
	{
		m_FileHandler.makeListAllFiles(root,true);
		vector<STDSTR> fileList =  m_FileHandler.getFilePaths();
		std::ofstream ostr(indexPath,ios::out);
		ostr.precision( 9 );         // 소수점 자릿수 9
		ostr.setf(ios_base:: fixed, ios_base:: floatfield); // 소수점 방식으로 표시됨
		int i=0;
		for(vector<STDSTR>::iterator iter=fileList.begin(); iter != fileList.end(); iter++) {
			char path[256] ={0,};
			string strPath(iter->begin(), iter->end());
			//cout << strPath << endl;
			Mat img = imread(strPath);
			//cout << "Read !" << endl;
			if(img.empty()) {
				cout << "not image file! "<< endl;
				continue;
			}
			else {
				//cout << "feature extract" << endl;
				baseExtract* color_histo = new ColorExtract();
				baseExtract* color_layout = new CLDExtract();
				baseExtract* edgeHistoD = new EHDExtract();
				baseExtract* homoTextureD = new homogeneousTextureExtract();
				
				//feature extract				
				colorExtract(img, (ColorExtract*) color_histo);
				cldExtract(img, (CLDExtract*) color_layout);
				ehdExtract(img, (EHDExtract*) edgeHistoD);
				htdExtract(img, (homogeneousTextureExtract*)homoTextureD);

				//add features
				feature_schema fs;
				fs.addFeature( *color_histo);
				fs.addFeature( *color_layout);
				fs.addFeature( *edgeHistoD);
				fs.addFeature( *homoTextureD);
				fs.setFileName(strPath);

				fs.write(ostr,(char*)strPath.c_str());
				m_feat_schemes.push_back(fs);
			
				img.release();
				//*/

				delete color_histo;
				delete color_layout;
				delete edgeHistoD;
				delete homoTextureD;
			}
			i++;
			cout << "Remains : " << fileList.size() - i << ": " << strPath << endl;
		}
		ostr.close();
		
	}
	feature_schema* getFeature_schema(int i) {
		try	{
			return & (m_feat_schemes[i]);
		}
		catch (Exception* e) {
			return NULL;
		}
		
	}
	baseExtract* getExtractedFeature(int i, const char* feature_name)
	{
		for(int j=0; j< getFeature_schema(i)->getFeatureSize(); j++)
			if(strcmp(getFeature_schema(i)->getFeatureName(j), feature_name) == 0)
				return getFeature_schema(i)->getFeaturePTR(j);

	}
	int getNFeatures() {
		return m_feat_schemes.size();
	}
};