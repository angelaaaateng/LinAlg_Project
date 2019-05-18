================================================================================

   TSR-Analysis
   
================================================================================
    
================================================================================
COMPILATION
================================================================================
Requirements
You need Qt 4.7 and a C++ Compiler to compile this project. There are no toolchain
specific makefiles/project files because this project relies on CMake (http://www.cmake.org).
  
1. Create and enter a directory where you want to build TSR-Analysis.
2. Invoke "cmake <path_to_TSR-Analysis_source_dir>" (you may need to specify the generator
   for your preferred toolchain, but usually cmake picks that up automatically.
   Alternative: Use the GUI cmake-gui (Set source and build directory, run "Configure" and "Generate")
3. Your build directory should now contain a makefile/project file for your favorite toolchain. 
   Do whatever you need to build TSR-Analysis.

   Make sure your Qt libraries accessible (e.g., in your path) when running TSR-Analysis


================================================================================
USAGE
================================================================================
Requirements
1. The image test set (see benchmark website)
2. The test set ground-truth (see benchmark website)
3. Your results. 

Settings
--------
When starting TSR-Analysis for the first time, you will be asked for a few settings.
These settings can be changed later in Settings->Path Settings (A restart may then be 
required to let those changes take effect. )

    - Pictures: path to the directory containing the test set
    - GT-data: path to CSV file with ground truth (as provided on benchmark website)


Loading results
---------------
On startup, you will be asked to load your results (CSV files). The files need to be in the same 
format as required by the benchmark website, ie:
  - no header
  - two columns, separated by semi-colon (";")
  - first column: image filname (without path, but with extension)
  - second column: assigned class
  - one entry per file in the test set

If you want to load multiple result files, please copy them to a single directory. 
Loading files via "File->Load CSVs" clears the previously loaded results.

Analysis  
--------
The program provides the following analysis features:

- Overall performance: 
    This is directly visible in the main window after results are loaded
    
- Misclassifications for single method: 
    If you click a button with a method name (the file name of the result file),
    a window open and shows the misclassifications of this method: The correct class,
    the actual image, and the classification result. The columns can be sorted
    by clicking the column header. Moving the mouse over the image reveals its
    filename.

- Inspect confusion matrix:
    After clicking "Confusion Matrix" a new window opens with the confusion matrix for the 
    corresponding approach. The rows represent ground-truth, the columns represent classifcation
    results. The confusion matrix allows further inspection of misclassifications.
    
    If you click into the matrix, a new window will show you the misclassified images: 
      - Cell: All images of class X that were classified as Y.
      - Row: All images of class X that were misclassified.
      - Column: All images that were incorrectly classified as class Y.
      - Top-left corner: All images that were misclassified.
      
    Unless you explicitly select a cell on the diagonal (correct classifications), the corresponding
    images are ignored and not shown (e.g. when you select a whole row).
      
    The confusion matrix allows to export/save the misclassified images. Just click "Save images" after selecting 
    a row, column, cell or whole matrix and you will be asked for a target directory. 
    If all incorrect images are saved, they just have the original file name (as in the test set).
    In all of the other cases, the filename is prefixed by the correct and recognized class, e.g., 
    "2-5-01234.ppm" says that 01234.ppm contains a sign of class 2 but was classified as 5.
    
- Comparison of multiple solutions:
    If multiple files are loaded, the results can be compared. Checking the "Compare" check box
    next to the result and clicking "Compare" below, opens a window similar to the one described 
    above for "Misclassifications for single method". However, this view contains one column per
    result that was included in the comparison. Blank entries in a column denote correct 
    classifications.
    
- Difficult signs:
    If multiple files are loaded, "Show->Difficult signs" shows the list of all incorrectly classified
    images, ordered and colored by number of misclassifications. This functions should tell you
    about the hard images (in the dataset or just for your approach). The view is similar to  
    "Misclassifications for single method". "Recognized" denotes the share of approaches that got a certain 
    image right. Example: If 4 result files are loaded, a "Recognized" value of 25% tells you that this
    image was misclassified in 3 out of those 4 cases (Use "Comparison of multiple solutions" for more 
    detailed insights).
    
================================================================================
LICENSE
================================================================================
This software is released under GNU Public License, Version 2. See gpl-2.0.txt for details.