#pragma once

#include "base_Extract.h"


class ColorExtract : public baseExtract
{
private:
	vector<Mat> bgr_planes;
	MatND  hist[3];
	const int blue;
	const int green;
	const int red;
public:
	ColorExtract() : baseExtract(), blue(0), green(1), red(2) {
		setFeatureName(STYLE_HISTO_COLOR);
		setDim(256);
	}
	~ColorExtract() {
		bgr_planes.clear();
		//cout << "Color Extractor Released!" << endl;
	}
	void saveXML() 
	{

	}
	void init_image() {
		cv::split(*m_img, bgr_planes);
	}
	void extract() {
		float range[] = {0, 16};
		const float* histRange[] = {range};
		//float histRange[1] = {256};
		init_image();
		calcHist(&bgr_planes[blue],1,0,Mat(),hist[blue],1,&m_nBins, histRange, true, false);
		calcHist(&bgr_planes[green],1,0,Mat(),hist[green],1,&m_nBins, histRange, true, false);
		calcHist(&bgr_planes[red],1,0,Mat(),hist[red],1,&m_nBins, histRange, true, false);
		float total = bgr_planes[blue].rows*bgr_planes[blue].cols;

		//normalize(hist[blue], hist[blue], 0.0, 1.0, NORM_MINMAX, -1, Mat() );
		//normalize(hist[green], hist[green], 0.0, 1.0, NORM_MINMAX, -1, Mat() );
		//normalize(hist[red], hist[red], 0, 1.0, NORM_MINMAX, -1, Mat() );

		for(int c=0; c<3; c++) {
			vector<float> descriptor;
			for( int h = 0; h < m_nBins; h++ )
			{
				float binVal = hist[c].at<float>(h)/total;
				descriptor.push_back(binVal);
			}
			m_descriptorsValues.push_back(descriptor);
		}
	}
	void showImage(char* windowName) {
		int hist_w = 512; int hist_h = 400;
		int bin_w = cvRound( (double) hist_w/m_nBins );

		Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

		/// Normalize the result to [ 0, histImage.rows ]
		normalize(hist[blue], hist[blue], 0, histImage.rows, NORM_MINMAX, -1, Mat() );
		normalize(hist[green], hist[green], 0, histImage.rows, NORM_MINMAX, -1, Mat() );
		normalize(hist[red], hist[red], 0, histImage.rows, NORM_MINMAX, -1, Mat() );

		/// Draw for each channel
		for( int i = 1; i < m_nBins; i++ )
		{
			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist[blue].at<float>(i-1)) ) ,
				Point( bin_w*(i), hist_h - cvRound(hist[blue].at<float>(i)) ),
				Scalar( 255, 0, 0), 2, 8, 0  );
			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist[green].at<float>(i-1)) ) ,
				Point( bin_w*(i), hist_h - cvRound(hist[green].at<float>(i)) ),
				Scalar( 0, 255, 0), 2, 8, 0  );
			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist[red].at<float>(i-1)) ) ,
				Point( bin_w*(i), hist_h - cvRound(hist[red].at<float>(i)) ),
				Scalar( 0, 0, 255), 2, 8, 0  );
		}

		/// Display
		namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
		imshow("calcHist Demo", histImage );

		waitKey(0);
	}
	virtual float dist(baseExtract* feat) {
		if(feat->typeCompare(this->getName())) {
			return -1.f;
		}
		ColorExtract* color = (ColorExtract*)feat;
		return L1Dist( (baseExtract&) (*feat ));
	}
	void generate(baseExtract* base) {
		vector<vector<float>> * dscr = base->getDscr();
	}
};

