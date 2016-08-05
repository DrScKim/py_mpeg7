#pragma once
#include "base_Extract.h"

#include "MPEG7_CV/Feature.h"
#include "MPEG7_CV/Frame.h"
#include "MPEG7_CV/Descriptors.h"
#include "MPEG7_CV/Extraction/EdgeHistExtraction.h"

#include "emd.h"
#include "ColoSpaceConverter.h"

using namespace XM;
inline float groundDist (feature_t *F1, feature_t *F2);
float groundDist (feature_t *F1, feature_t *F2)
{
	int luv1[3], luv2[3];
	ColorSpaceConverter* csc = new ColorSpaceConverter();
	csc->rgb2luv(F1->R, F1->G, F1->B, luv1);
	csc->rgb2luv(F2->R, F2->G, F2->B, luv2);
	float dist = pow((float)(luv1[0]-luv2[0])/256.f,2)+pow((float)(luv1[1]-luv2[1])/256.f,2)+pow((float)(luv1[2]-luv2[2])/256.f,2);
	return sqrt(dist);

}

class MPEG7_EXTRACT
{
protected:
	Mat* m_img;
	Frame* m_frame;
	Feature m_MPEG7_Feature;
	void setFrame(string filePath) {
		Mat img = imread(filePath, CV_LOAD_IMAGE_COLOR);
		ref_Image(&img);
		m_frame = new Frame(m_img->rows, m_img->cols, true, false, false);
		m_frame->setImage( (Mat&) *m_img);
	}
	void setFrame(Mat* img) {
		ref_Image(img);
		m_frame = new Frame(m_img->rows, m_img->cols, true, false, false);
		m_frame->setImage((Mat&) *m_img);
	}
	void ref_Image(Mat* img) {
		m_img = img;
	}
public:
	MPEG7_EXTRACT() : m_frame(NULL) {}
	~MPEG7_EXTRACT() {
		if(!m_frame)
			delete m_frame;
	}
};

class homogeneousTextureExtract : public baseExtract
{
	Frame* m_frame;
	Feature m_MPEG7_Feature;
	XM::HomogeneousTextureDescriptor* m_htd;

	void setFrame(Mat* img) {
		ref_Image(img);
		m_frame = new Frame(m_img->rows, m_img->cols, true, true, false);
		m_frame->setImage((Mat&) *m_img);
	}
public:
	homogeneousTextureExtract() : baseExtract(), m_frame(NULL), m_htd(NULL) {
		setFeatureName(STYLE_MPEG7_HTD);
	}
	~homogeneousTextureExtract() {
		if(m_frame != NULL)
			delete m_frame;
		if(m_htd != NULL)
			delete m_htd;
	}
	XM::HomogeneousTextureDescriptor* getDescriptor() {
		return m_htd;
	}
	virtual void extract() {
		setFrame(m_img);
		
		Mat gray;
		cvtColor(*m_img, gray, CV_BGR2GRAY);
		m_frame->setGray(gray);
		if(m_frame != NULL) {
			m_htd = Feature::getHomogeneousTextureD(m_frame);
		}

		vector<float> descriptor;
		for(int i=0; i<62; i++) {
			descriptor.push_back( m_htd->feature[i]);	
		}
		m_descriptorsValues.push_back(descriptor);
		vector<float> flag;
		flag.push_back(m_htd->energydeviationflag);
		m_descriptorsValues.push_back(flag);
	}
	void generate(baseExtract* base) {
		vector<vector<float>> * dscr = base->getDscr();
		vector<float> dscrs = (*dscr)[0];
		int* feat = new int[62]; bool flag=(*dscr)[1][0];
		this->m_descriptorsValues.push_back(dscrs);
		if(m_htd == NULL) {
			m_htd = new HomogeneousTextureDescriptor();
		}
		for(int i=0; i<62; i++) {
			feat[i] = (int)dscrs[i];
		}
		m_htd->resetDescriptor();
		m_htd->SetHomogeneousTextureFeature(flag, feat);
		delete [] feat;
	}
	virtual float dist(baseExtract* feat) {
		if(!feat->typeCompare(this->getName())) {
			return -1.f;
		}
		homogeneousTextureExtract* htd = (homogeneousTextureExtract*)feat;
		return m_htd->distance(htd->getDescriptor());

	}
};

class DCDExtract : public baseExtract
{
private:
	Frame* m_frame;
	Feature m_MPEG7_Feature;
	XM::DominantColorDescriptor* m_dcd;

	signature_t getSignature(XM::DominantColorDescriptor* dcd)
	{
		feature_t *feat = new feature_t[dcd->GetDominantColorsNumber()];
		float* weight = new float[dcd->GetDominantColorsNumber()];
		for(int i=0; i < dcd->GetDominantColorsNumber() ; i++)
		{
			feat[i].R = dcd->GetDominantColors()[i].m_ColorValue[0];
			feat[i].G = dcd->GetDominantColors()[i].m_ColorValue[1];
			feat[i].B = dcd->GetDominantColors()[i].m_ColorValue[2];
			weight[i] = float(dcd->GetDominantColors()[i].m_Percentage);
		}
		signature_t sig = {dcd->GetDominantColorsNumber(), feat, weight};
		return sig;
	}

	float emd_dcd_dist(XM::DominantColorDescriptor* dcd) 
	{
		signature_t sig1 = getSignature(this->m_dcd);
		signature_t sig2 = getSignature(dcd);
		float dist = emd(&sig1, &sig2, groundDist, 0, 0);
		return dist;
	}
	void setFrame(Mat* img) {
		ref_Image(img);
		m_frame = new Frame(m_img->rows, m_img->cols, true, false, false);
		m_frame->setImage((Mat&) *m_img);
	}
	int bin1, bin2, bin3;
public:
	DCDExtract() :baseExtract(), m_frame(NULL), m_dcd(NULL), bin1(32), bin2(32), bin3(32){
		setFeatureName(STYLE_MPEG7_DCD);
	}
	~DCDExtract() {
		if(NULL != m_frame) delete m_frame;
		if(NULL != m_dcd) delete m_dcd;
	}
	XM::DominantColorDescriptor* getDescriptor() {
		return m_dcd;
	}

	virtual float dist(baseExtract* feat) {
		
		if(!feat->typeCompare(this->getName())) {
			return -1.f;
		}
		
		DCDExtract* dcd = (DCDExtract*)feat;
		return emd_dcd_dist(dcd->getDescriptor());
	}
	
	void setBinNum(int b1, int b2, int b3) {bin1=b1; bin2=b2; bin3=b3;}
	int getBin(int i) {
		if( i == 0) return bin1;
		if( i == 1) return bin2;
		if( i == 2) return bin3;
	}
	virtual void extract() {
		setFrame(m_img);
		printf("set frame\n");
		if(NULL != m_frame) {
			m_dcd = Feature::getDominantColorD(m_frame, false);
		}

		vector<float> percent_descriptor;
		vector<float> colorVal_descriptor;
		vector<float> colorVar_descriptor;
		printf("m_dcd->GetDominantColorsNumber() : %d\n", m_dcd->GetDominantColorsNumber());
		for(int i=0; i<m_dcd->GetDominantColorsNumber(); i++) {
			XM::DOMCOL * dc = m_dcd->GetDominantColors();
			percent_descriptor.push_back(dc[i].m_Percentage);
			colorVal_descriptor.push_back(dc[i].m_ColorValue[0]);
			colorVal_descriptor.push_back(dc[i].m_ColorValue[1]);
			colorVal_descriptor.push_back(dc[i].m_ColorValue[2]);
			colorVar_descriptor.push_back(dc[i].m_ColorVariance[0]);
			colorVar_descriptor.push_back(dc[i].m_ColorVariance[1]);
			colorVar_descriptor.push_back(dc[i].m_ColorVariance[2]);
		}
		
		m_descriptorsValues.push_back(percent_descriptor);
		m_descriptorsValues.push_back(colorVal_descriptor);
		m_descriptorsValues.push_back(colorVar_descriptor);
	}
	void generate(baseExtract* base) {
		vector<vector<float>> * dscr = base->getDscr();
		vector<float> percentages = (*dscr)[0];
		vector<float> values =  (*dscr)[1];
		vector<float> vars =  (*dscr)[2];
		int size = percentages.size();
		m_dcd = new XM::DominantColorDescriptor();
		m_dcd->resetDescriptor();
		int* percs = new int[size];
		int** colors = new int*[size];
		int** variances = new int*[size];
		for(int i=0; i<size; i++) {
			colors[i] = new int[3];
			variances[i] = new int[3];
		}
		for(int i=0; i<size; i++) {
			percs[i] = percentages[i];
			colors[i][0] = values[i*3];
			colors[i][1] = values[i*3+1];
			colors[i][2] = values[i*3+2];
			variances[i][0] = vars[i*3];
			variances[i][1] = vars[i*3+1];
			variances[i][2] = vars[i*3+2];
		}
		m_dcd->SetDominantColorsNumber(size);
		m_dcd->SetDominantColors(percs, colors, variances);
		
		for(int i=0; i<size; i++) {
			delete [] *(colors+i);
			delete [] *(variances+i);
		}
		delete [] percs;
		delete [] colors;
		delete [] variances;
	}
};


class CSDExtract : public baseExtract
{
private:
	Frame* m_frame;
	Feature m_MPEG7_Feature;
	XM::ColorStructureDescriptor* m_csd;

	void setFrame(Mat* img) {
		ref_Image(img);
		m_frame = new Frame(m_img->rows, m_img->cols, true, false, false);
		m_frame->setImage((Mat&) *m_img);
	}
public:
	CSDExtract() : baseExtract(), m_frame(NULL), m_csd(NULL) {
		setFeatureName(STYLE_MPEG7_CSD);
	}
	~CSDExtract() {
		if(m_frame != NULL)
			delete m_frame;
		if(m_csd != NULL)
			delete m_csd;
	}
	XM::ColorStructureDescriptor* getDescriptor() {
		return m_csd;
	}
	virtual void extract() {
		setFrame(m_img);
		if(m_frame != NULL) {
			m_csd = Feature::getColorStructureD(m_frame);
		}

		vector<float> descriptor;
		
		for(int i=0; i<m_csd->GetSize(); i++) {
			descriptor.push_back( m_csd->GetElement(i));	
		}
		m_descriptorsValues.push_back(descriptor);
	}
	virtual float dist(baseExtract* feat) {
		if(!feat->typeCompare(this->getName())) {
			return -1.f;
		}
		CSDExtract* csd = (CSDExtract*)feat;
		return m_csd->distance(csd->getDescriptor());
	}
};

class EHDExtract : public baseExtract
{
private:
	Frame* m_frame;
	Feature m_MPEG7_Feature;
	XM::EdgeHistogramDescriptor* m_ehd;

	void setFrame(Mat* img) {
		ref_Image(img);
		m_frame = new Frame(m_img->rows, m_img->cols, true, false, false);
		m_frame->setImage((Mat&) *m_img);
	}
public:
	EHDExtract() : baseExtract(), m_frame(NULL), m_ehd(NULL){
		setFeatureName(STYLE_MPEG7_EHD);
	}
	~EHDExtract() {
		if(m_frame != NULL)
			delete m_frame;
		if(m_ehd != NULL)
			delete m_ehd;
	}
	XM::EdgeHistogramDescriptor* getDescriptor() {
		return m_ehd;
	}
	void generate(baseExtract* base) {
		vector<vector<float>> * dscr = base->getDscr();
		XM::EHD ehd;
		vector<float> dscrs;
		for(int i=0; i< (*dscr)[0].size(); i++) {
			ehd.Local_Edge[i] = (double)((*dscr)[0][i]);
			dscrs.push_back(((*dscr)[0][i]));
		}
		this->m_descriptorsValues.push_back(dscrs);
		if(m_ehd == NULL) {
			m_ehd = new EdgeHistogramDescriptor();
		}
		m_ehd->resetDescriptor();
		m_ehd->SetEdgeHistogram(&ehd);
	}
	virtual void extract() {
		setFrame(m_img);
		if(m_frame != NULL) {
			m_ehd = Feature::getEdgeHistogramD(m_frame);
		}

		vector<float> descriptor;
		for(int i=0; i<80; i++) {
			descriptor.push_back( (float) ( m_ehd->GetEdgeHistogram()->Local_Edge[i]));	
		}
		m_descriptorsValues.push_back(descriptor);
	}
	virtual float dist(baseExtract* feat) {
		if(!feat->typeCompare(this->getName())) {
			return -1.f;
		}
		EHDExtract* ehd = (EHDExtract*)feat;
		return m_ehd->distanceNorm(ehd->getDescriptor());
	}
};

class CLDExtract : public baseExtract
{
private:
	Frame* m_frame;
	Feature m_MPEG7_Feature;
	XM::ColorLayoutDescriptor* m_cld;

	void setFrame(Mat* img) {
		ref_Image(img);
		m_frame = new Frame(m_img->rows, m_img->cols, true, false, false);
		m_frame->setImage((Mat&) *m_img);
	}
public:
	XM::ColorLayoutDescriptor* getDescriptor() {
		return m_cld;
	}
	CLDExtract() : baseExtract(), m_frame(NULL), m_cld(NULL) {
		setFeatureName(STYLE_MPEG7_CLD);
		
	}
	~CLDExtract() {
		if(m_frame != NULL)
			delete m_frame;
		if(m_cld != NULL)
			delete m_cld;
	}

	void generate(baseExtract* base) {
		vector<vector<float>> * dscr = base->getDscr();
		
		if(m_cld == NULL) {
			m_cld = new ColorLayoutDescriptor();
		}

		int* YCoff = new int[(*dscr)[0].size()];
		int* CBCoff = new int[(*dscr)[1].size()];
		int* CRCoff = new int[(*dscr)[2].size()];

		vector<float> y;
		vector<float> cb;
		vector<float> cr;

		for(int i=0; i< (*dscr)[0].size(); i++) {
			YCoff[i] = (*dscr)[0][i];
			y.push_back(YCoff[i]);
		}
		for(int i=0; i< (*dscr)[1].size(); i++) {
			CBCoff[i] = (*dscr)[1][i];
			cb.push_back(CBCoff[i]);
		}
		for(int i=0; i< (*dscr)[2].size(); i++) {
			CRCoff[i] = (*dscr)[2][i];
			cr.push_back(CRCoff[i]);
		}
		this->m_descriptorsValues.push_back(y);
		this->m_descriptorsValues.push_back(cb);
		this->m_descriptorsValues.push_back(cr);
		m_cld->SetNumberOfCCoeff((*dscr)[1].size());
		m_cld->SetNumberOfYCoeff((*dscr)[0].size());
		m_cld->SetYCoeff(YCoff);
		m_cld->SetCrCoeff(CRCoff);
		m_cld->SetCbCoeff(CBCoff);

	}

	virtual void extract() {
		setFrame(m_img);
		if(m_frame != NULL) {
			m_cld = Feature::getColorLayoutD(m_frame);
		}
		vector<float> Ydescriptor;
		vector<float> CBdescriptor;
		vector<float> CRdescriptor;
		int *Ycoff = m_cld->GetYCoeff();
		int *CBcoff = m_cld->GetCbCoeff();
		int *CRcoff = m_cld->GetCrCoeff();
		for(int i=0; i < m_cld->GetNumberOfYCoeff(); i++ ) {
			Ydescriptor.push_back((float) Ycoff[i]);
		}
		for(int i=0; i< m_cld->GetNumberOfCCoeff(); i++) {
			CBdescriptor.push_back((float) CBcoff[i]);
			CRdescriptor.push_back((float) CRcoff[i]);
		}
		m_descriptorsValues.push_back(Ydescriptor);
		m_descriptorsValues.push_back(CBdescriptor);
		m_descriptorsValues.push_back(CRdescriptor);
	}
	virtual float dist(baseExtract* feat) {
		if(!feat->typeCompare(this->getName())) {
			return -1.f;
		}
		CLDExtract* cld = (CLDExtract*)feat;
		return m_cld->distanceNorm(cld->getDescriptor());
	}

};