#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

/** @function main */
int main( int argc, const char** argv )
{
    if (argc != 2){
        
        cout << "--(!)Usage error !\n Usage :  " <<argv[0]<<" your_path_to_image \n";
        return -1;
    }
    String face_cascade_name = samples::findFile( "./data/haarcascades/haarcascade_frontalface_alt.xml" );
    String eyes_cascade_name = samples::findFile( "./data/haarcascades/haarcascade_eye_tree_eyeglasses.xml" );

    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
    if( !eyes_cascade.load( eyes_cascade_name ) )
    {
        cout << "--(!)Error loading eyes cascade\n";
        return -1;
    };
    Mat frame = imread(argv[1]);
    if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
        }

        //-- 3. Apply the classifier to the frame
        detectAndDisplay( frame );
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    std::vector<Rect> faces;
    face_cascade.detectMultiScale( frame_gray, faces );

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        int fh =faces[i].height - (faces[i].height * 0.2); // We want something more cropped than the default values
        int fw = faces[i].width - (faces[i].width * 0.3); 
        int fx =faces[i].x + (faces[i].width * 0.15); 
        int fy = faces[i].y + (faces[i].height * 0.1);
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        
        ellipse( frame, center, Size( 1, 1 ), 0, 0, 360, Scalar( 0, 0, 255 ), 6 );
        cout <<i<<"-fh="<<fh<<"-fw="<<fw<<"-fx="<<fx<<"-fy="<<fy<<"\n";
        Mat croppedFace = frame(Rect(fx,fy,fw,fh)); 
        imwrite("./img/cropped"+std::to_string(i)+".jpg",croppedFace);
        
        
        // Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 255, 255 ), 4 );
        Mat faceROI = frame_gray( faces[i] );
        
        //-- In each face, detect eyes
        std::vector<Rect> eyes;
        eyes_cascade.detectMultiScale( faceROI, eyes );

        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4 );
        }
    }

    //-- Show what you got
    namedWindow("Capture - Face detection", WINDOW_NORMAL);
    resizeWindow("Capture - Face detection", 1200, 700);// to fix issues where the image is bigger than the screen
    imshow( "Capture - Face detection", frame );
    waitKey(0);

}
