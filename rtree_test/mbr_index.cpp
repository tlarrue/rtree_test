//
//  mbr_index.cpp
//  rtree_test
//
//  Created by Tara Larrue on 5/7/17.
//  Copyright © 2017 Tara Larrue. All rights reserved.
//

#include "mbr_index.hpp"

/* ****************************** */
/*             MACROS             */
/* ****************************** */

#ifdef TILEDB_VERBOSE
#  define PRINT_ERROR(x) std::cerr << TILEDB_MI_ERRMSG << x << ".\n"
#else
#  define PRINT_ERROR(x) do { } while(0)
#endif

/* ********************************* */
/*          GLOBAL VARIABLES         */
/* ********************************* */

/** Stores potential error messages. */
std::string tiledb_mi_errmsg = "";

/*
 Constructor
 */
MbrIndex::MbrIndex(){
    dim_num_ = 0;
}

/*
 Populate Rtree index with bulkloading
 */
int MbrIndex::buildTree(std::vector<void*> &mbrs, int dim){
 
    try {
        if (dim == 2){
            dim_num_=2;
            build2d(mbrs);
        } else if (dim == 3){
            dim_num_=3;
            build3d(mbrs);
        } else {
            std::string errmsg = "Only 2 or 3 dimensions supported by MbrIndex.";
            PRINT_ERROR(errmsg);
            tiledb_mi_errmsg = TILEDB_MI_ERRMSG + errmsg;
            return TILEDB_MI_ERR;
        }
    }
    catch (...){
        return TILEDB_MI_ERR;
    }
    
    // Success
    return TILEDB_MI_OK;
}

/*
 Return index of MBRs that intersect with a subarray
 */
int MbrIndex::intersectQuery(const double* subarray, std::vector<int64_t> &result){
    
    try {
        if (dim_num_ == 2){
            intersect2d(subarray, result);
        } else if (dim_num_ == 3){
            intersect3d(subarray, result);
        } else {
            std::string errmsg = "Only 2 or 3 dimensions supported by MbrIndex.";
            PRINT_ERROR(errmsg);
            tiledb_mi_errmsg = TILEDB_MI_ERRMSG + errmsg;
            return TILEDB_MI_ERR;
        }
        
    } catch (...){
        return TILEDB_MI_ERR;
    }
    
    // Success
    return TILEDB_MI_OK;
}

/*
 Build a tree with 2D points
 */
void MbrIndex::build2d(std::vector<void*> &mbrs){
    
    std::vector<value2d> pairs;
    pairs.resize(mbrs.size());
    
    // MBR = [minX, minY, maxX, maxY]
    for (unsigned long i=0; i<mbrs.size(); i++){
        
        const double* mbr = static_cast<const double*>(mbrs[i]);
        
        box2d b = box2d(point2d(mbr[0], mbr[2]), point2d(mbr[1], mbr[3]));
        pairs[i] = std::make_pair(b,i);
    }

    pack_rtree2d rtree(pairs);
    rtree2d_ = rtree;
}

/*
 Build a tree with 3D points
 */
void MbrIndex::build3d(std::vector<void*> &mbrs){
    
    std::vector<value3d> pairs;
    pairs.resize(mbrs.size());
    
    // MBR = [minX, minY, maxX, maxY, minZ, maxZ]
    for (unsigned long i=0; i<mbrs.size(); i++){
        
        const double* mbr = static_cast<const double*>(mbrs[i]);

        box3d b = box3d(point3d(mbr[0], mbr[2], mbr[4]), point3d(mbr[1], mbr[3], mbr[5]));
        pairs[i] = std::make_pair(b,i);
    }
    
    pack_rtree3d rtree(pairs);
    rtree3d_ = rtree;
}

/*
 Perform Intersect Query with 2d coordinates
 */
void MbrIndex::intersect2d(const double* subarray, std::vector<int64_t> &result){
    
    //convert subarray to box to use boost functions
    box2d query_box(point2d(subarray[0], subarray[2]), point2d(subarray[1], subarray[3]));
    std::vector<value2d> q;

    rtree2d_.query(bgi::intersects(query_box), std::back_inserter(q));

    for (int i=0; i<q.size(); i++){
        result.push_back(q[i].second);
    }
}

/*
 Perform Intersect Query with 3d coordinates
 */
void MbrIndex::intersect3d(const double* subarray, std::vector<int64_t> &result){

    //convert subarray to box to use boost functions
    box3d query_box(point3d(subarray[0], subarray[2], subarray[4]), point3d(subarray[1], subarray[3], subarray[5]));
    std::vector<value3d> q;
    
    rtree3d_.query(bgi::intersects(query_box), std::back_inserter(q));
    
    for (int i=0; i<q.size(); i++){
        result.push_back(q[i].second);
    }
}
