## C++ Implementation For B-snake Lane Detection

This is the code for lane detection based on the bsnake paper by Wang et al.

## What is B-Snake based lane model
- It described the perspective effect of parallel lines is constructed with dual external forces for generic lane boundary or marking
- It is able to describe a wider range of lane structures than other lane models such as straight and parabolic models.
- It is robust against shadows, noises and more due to the use of parallel knowledge of roads on the ground plane.
- The lane detection problem is formulated by determining the set of lane model control points.

## Canny/Hough Estimation of Vanishing Points (CHEVP) Algorithm
- It is a robust algorithm presented for providing a good initial position for B-Snake lane Model which robust to noises, shadows, and illumination variations in the captured road images.
- It is also applicable to both marked and unmarked, dash paint line and solid paint line roads.

## How CHEVP algorithm Works
- The road is assumed to have two parallel boundaries on the ground, and in the short horizontal band of image, the road is approximately straight. As a result of the perspective projection, the road boundaries in the image plane should intersect at a shared vanishing point on the horizon. 

- There are five processing stages in CHEVP algorithms:
1. Edge pixel extraction by Canny edge detection. Canny edge detection is employed to obtain edge map.

2. Straight lines detection by hough transform.

3. Horizon and vanishing detection.

4. Estimation the mid-line of road and the parameter k by the detected road lines.
```
  $$k=c_right-c_left/r_mid-hz$$
```
hz = vertical coordinate of the vanishing line

5. Initial the control points of the lane model to approach the mid-line detected by last step.


### Reference
http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.106.6644&rep=rep1&type=pdf
