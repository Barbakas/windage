/* ========================================================================
 * PROJECT: windage Library
 * ========================================================================
 * This work is based on the original windage Library developed by
 *   Woonhyuk Baek
 *   Woontack Woo
 *   U-VR Lab, GIST of Gwangju in Korea.
 *   http://windage.googlecode.com/
 *   http://uvr.gist.ac.kr/
 *
 * Copyright of the derived and new portions of this work
 *     (C) 2009 GIST U-VR Lab.
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact 
 *   Woonhyuk Baek
 *   <windage@live.com>
 *   GIST U-VR Lab.
 *   Department of Information and Communication
 *   Gwangju Institute of Science and Technology
 *   1, Oryong-dong, Buk-gu, Gwangju
 *   South Korea
 * ========================================================================
 ** @author   Woonhyuk Baek
 * ======================================================================== */

#ifndef _MODIFIED_SURF_TRACKER_H_
#define _MODIFIED_SURF_TRACKER_H_

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#include <vector>
#include <cv.h>
#include <cvaux.h>

#include "Tracker.h"
#include "OpticalFlow.h"
#include "Utils/Logger.h"

#include "FAST/fast.h"
#include "FAST/wsurf.h"
#include "FAST/wfastsurf.h"

namespace windage
{

	/**
	 * @brief
	 *		Class for Camera Tracker using natural feature
	 * @author
	 *		windage
	 */
	class DLLEXPORT ModifiedSURFTracker : public Tracker
	{
	private:
		double realWidth;	///< reference image information
		double realHeight;	///< reference image information
		int featureExtractThreshold;	///< FAST Corner extract threshold value

		IplImage* referenceImage;	///< attatched reference image
		IplImage* prevImage;
		std::vector<SURFDesciription> referenceSURF;	///< reference image surf description
		std::vector<SURFDesciription> sceneSURF;		///< input image surf description

		std::vector<SURFDesciription> matchedReference;		///< optical flow matched reference point
		std::vector<SURFDesciription> matchedScene;			///< optical flow matched scene point
		std::vector<CvPoint2D32f> matchedReferencePoints;
		std::vector<CvPoint2D32f> matchedScenePoints;

		void Release();

		CvMat* referenceFeatureStorage;			///< reference feature storage
		CvFeatureTree* referenceFeatureTree;	///< reference feature tree for k-d tree

		cv::Mat flannFeatureTree;
		cv::flann::Index* flannIndex;

		int GenerateReferenceFeatureTree(double scaleFactor=4.0, int scaleStep=8);
		CvFeatureTree* CreateReferenceTree(std::vector<SURFDesciription>* referenceSURF, CvMat* referenceFeatureStorage);
		bool CreateFlannTree(std::vector<SURFDesciription>* referenceSURF, CvMat* referenceFeatureStorage);

		int FindPairs(SURFDesciription description, CvFeatureTree* tree, double distanceRate, float* outDistance = NULL);
		int FindPairs(SURFDesciription description, std::vector<SURFDesciription>* descriptions);
		int FindPairs(SURFDesciription description, cv::flann::Index* treeIndex, float distanceRate=0.7, float* outDistance = NULL);

		POSE_ESTIMATION_METHOD poseEstimationMethod;
		bool outlinerRemove; 

		/**
		 * @brief
		 *		Calculate Pose
		 * @remark
		 *		Calculate pose and update using reference and scene surf description
		 */
		double CalculatePose(
								bool update=true	///< apply the camera paramter (extrinsic parameter)
							);

		// optical flow
		bool runOpticalflow;
		unsigned int stepSize;
		unsigned int step;
		OpticalFlow* opticalflow;

	public:
		ModifiedSURFTracker();
		virtual ~ModifiedSURFTracker();

		inline void SetSetpIndex(int index){this->step = index;};

		inline double GetRealWidth(){return this->realWidth;};
		inline double GetRealHeight(){return this->realHeight;};

		inline int GetFeatureCount(){return sceneSURF.size();};
		inline int GetMatchedCount(){return matchedScene.size();};

		inline IplImage* GetReferenceImage(){return this->referenceImage;};
		inline int GetFeatureExtractThreshold(){return this->featureExtractThreshold;};
		inline void SetFeatureExtractThreshold(int threshold=80){this->featureExtractThreshold = threshold;};

		inline void SetPoseEstimationMethod(POSE_ESTIMATION_METHOD poseEstimationMethod=windage::RANSAC){this->poseEstimationMethod = poseEstimationMethod;};
		inline POSE_ESTIMATION_METHOD GetPoseEstimationMethod(){return this->poseEstimationMethod;};
		inline void SetOutlinerRemove(bool remove){this->outlinerRemove = remove;};

		/**
		 * @brief
		 *		Initialize ModifiedSURF Tracker
		 * @remark
		 *		initialize ModifiedSURF tracker setting FAST corner threshold and camera paramter (intrinsic parameter)
		 */
		void Initialize(double fx, 				///< intrinsic parameter x focal length
						double fy, 				///< intrinsic parameter y focal length
						double cx, 				///< intrinsic parameter x principle point
						double cy, 				///< intrinsic parameter y principle point
						double d1, 				///< intrinsic parameter distortion factor1
						double d2, 				///< intrinsic parameter distortion factor2
						double d3, 				///< intrinsic parameter distortion factor3
						double d4, 				///< intrinsic parameter distortion factor4
						int featureExtractThreshold=100	///< FAST corner extract threshold
						);
		
		/**
		 * @brief
		 *		regist reference image
		 * @remark
		 *		regist reference image
		 */
		void RegistReferenceImage(	IplImage* referenceImage,	///< reference image
									double realWidth,			///< image real width size
									double realHeight,			///< image real width size
									double scaleFactor=4.0,		///< scale factor for multi-scale
									int scaleStep=8				///< multi-scale step
									);

		/**
		 * @brief
		 *		Extract FAST Corner
		 * @remark
		 *		static method to extract fast corner points
		 */
		static int ExtractFASTCorner(
									std::vector<CvPoint>* corners,	///< output corner points
									IplImage* grayImage,			///< input image
									int threshold,					///< FAST Corner threshold
									int n=9							///< 9 ~ 12 : continuous count
									);

		/**
		 * @brief
		 *		Extract ModifiedSURF
		 * @remark
		 *		static method to generate modified surf descriptor using input points
		 */
		static int ExtractModifiedSURF(
										IplImage* grayImage,						///< input image
										std::vector<CvPoint>* corners,				///< input corners
										std::vector<SURFDesciription>* descriptions	///< output descriptors
										);

		/**
		 * @brief
		 *		Update Camera Pose (extrinsic parameter)
		 * @remark
		 *		update extrinsic parameter method using input image
		 */
		double UpdateCameraPose(IplImage* grayImage);

		/**
		 * @brief
		 *		Draw Debug Information
		 * @remark
		 *		draw debug information method
		 */
		void DrawDebugInfo(IplImage* colorImage);
		void DrawDebugInfo2(IplImage* colorImage);
		void DrawOutLine(IplImage* colorImage, bool drawCross = false);

		// optical flow
		/**
		 * @brief
		 *		Initialize OpticalFlow
		 * @remark
		 *		initialize OpticalFlow setting pyramid level and window size
		 *		running opticalflow after initialization
		 */
		void InitializeOpticalFlow(
									int width,							///< input image width size
									int height,							///< input image height size
									int stepSize=10,					///< calling time to recognize object
									CvSize windowSize=cvSize(10, 10),	///< opticalflow window size
									int pyramidLevel=3					///< opticalflow pyramid level
									);
		inline void SetOpticalFlowRunning(bool run){this->runOpticalflow = run;};

		Logger* log;
	};
}

#endif