This folder contains our work regarding Haar cascade classifier. We needed this classifier to get the faces of the images we want to compare in order to be more precise during our comparisons.

To use our program, simply compile using "make", and run it as it follows : "./objectDetection your_path_to_image"

We used OpenCV4 implementation of the cascade classifier.
To install OpenCV we've followed this tutorial : https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html

When the installation is done, it is possible to encounter compiling error because the compilor can't find files located in "opencv2".

To solve this we use the linker pkg, see our makefile.

To learn how to use Haar classifier, we've followed this tutorial (hence the similarities in our code) : https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html