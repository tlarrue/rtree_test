//
//  main.cpp
//  rtree_test
//
//  Created by Tara Larrue on 5/7/17.
//  Copyright © 2017 Tara Larrue. All rights reserved.
//

#include <iostream>
#include <stdio.h>

#include "mbr_index.hpp"

int main(int argc, const char * argv[]) {

    // build mbr index
    int dim = 2;
    MbrIndex* mbr_index_= new MbrIndex();

    // vector of T[]* arrays
    // array = [minX, maxX, minY, maxY]
    std::vector<void*> mbrs_;

    double testData[] = {
      0.000038,0.655671,		0.317673,178.823425,
      0.656033,1.310651,		0.318130,179.260391,
      1.311005,1.980705,		1.270584,179.414124,
      1.981313,2.624069,		1.575615,178.893402,
      2.624133,3.293271,		0.789871,179.268768,
      3.293281,3.981244,		0.516144,179.758987,
      3.981270,4.688187,		0.608994,179.048828,
      4.688283,5.375754,		0.544922,179.469238,
      5.375938,6.066377,		0.463280,179.549011,
      6.066381,6.755997,		0.205704,179.515701,
      6.756030,7.473213,		0.232231,179.525421,
      7.473274,8.194122,		0.881592,179.721313,
      8.194196,8.918003,		0.709015,179.425446,
      8.918122,9.638528,		1.016159,179.264771,
      9.638568,10.379612,		1.030876,179.438980,
      10.379699,11.120518,		0.332314,179.322006,
      11.120545,11.870239,		0.512352,179.051392,
      11.870887,12.629448,		0.897636,179.230774,
      12.629853,13.387108,		0.526588,179.287003,
      13.387207,14.145905,		0.232880,179.087616,
      14.146108,14.887004,		0.762390,179.245605,
      14.887941,15.637600,		0.628227,179.239594,
      15.638074,16.399275,		0.976257,179.233780,
      16.399611,17.177870,		0.507240,179.636292
    };

    int testSize = sizeof(testData) / sizeof(testData[0]);

    for (int i=0; i<testSize/4; i++){
        /* double* mbr = new double[4]; */
        /* for (int j=0; j<4; ++j) */
        /*   mbr[j] = testData[i*4+j]; */
        /* mbrs_.push_back((void*) mbr); */
      mbrs_.push_back((void*) &(testData[i*4]));
    }

    mbr_index_->buildTree(mbrs_, dim);

    // perform intersect query
    double subarray[] = {
        0.0, 10.0, 0.0, 10.0
    };

    std::vector<int64_t> search_tile_candidates_;
    const double* subarray2 = static_cast<const double*>((void*)&subarray);
    mbr_index_->intersectQuery(subarray2, search_tile_candidates_);

    // print results
    std::cout << "Result size: " << search_tile_candidates_.size() << std::endl;
    for (int i=0; i<search_tile_candidates_.size(); i++){
        std::cout << search_tile_candidates_[i] << std::endl;
    }

    return 0;
}
