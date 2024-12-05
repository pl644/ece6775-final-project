//==========================================================================
// digitrec.cpp
//==========================================================================
// @brief: A k-nearest-neighbors implementation for digit recognition

#include "digitrec.h"

//----------------------------------------------------------
// Top function
//----------------------------------------------------------
// @param[in] : input - the testing instance
// @return : the recognized digit (0~9)

bit4 digitrec(digit input) {
#include "training_data.h"

  // This array stores K minimum distances per training set
  bit6 knn_set[10][K_CONST];

  // Initialize the knn set
  knn_init_outer: for (int i = 0; i < 10; ++i)
    knn_init_inner: for (int k = 0; k < K_CONST; ++k)
      // Note that the max distance is 49
      knn_set[i][k] = 50;

  training_outer: for (int i = 0; i < TRAINING_SIZE; ++i) {
    training_inner: for (int j = 0; j < 10; j++) {

      // Read a new instance from the training set
      digit training_instance = training_data[j * TRAINING_SIZE + i];
      // Update the KNN set
      update_knn(input, training_instance, knn_set[j]);
    }
  }

  // Compute the final output
  return knn_vote(knn_set);
}

//-----------------------------------------------------------------------
// update_knn function
//-----------------------------------------------------------------------
// Given the test instance and a (new) training instance, this
// function maintains/updates an array of K minimum
// distances per training set.

// @param[in] : test_inst - the testing instance
// @param[in] : train_inst - the training instance
// @param[in/out] : min_distances[K_CONST] - the array that stores the current
//                  K_CONST minimum distance values per training set

void update_knn(digit test_inst, digit train_inst,
                bit6 min_distances[K_CONST]) {

  // Compute the bitwise difference between two digits using XOR
  digit diff = test_inst ^ train_inst;
  bit6 dist = 0;
  // Calculate the hamming distance by counting the number of 1s
  popcount: for (int i = 0; i < 49; ++i) {
    dist += diff[i];
  }

  // Update the KNN set
  for (int k = 0; k < K_CONST; ++k) {
    if (dist < min_distances[k]) {
      for (int j = K_CONST - 1; j > k; --j) {
        min_distances[j] = min_distances[j - 1];
      }
      min_distances[k] = dist;
      break;
    }
  }
}


// ... (sort_knn function remains unchanged) ...

//-----------------------------------------------------------------------
// knn_vote function
//-----------------------------------------------------------------------
// Given 10xK minimum distance values, this function
// finds the actual K nearest neighbors and determines the
// final output based on the most common digit represented by
// these nearest neighbors (i.e., a vote among KNNs).
//
// @param[in] : knn_set - 10xK_CONST min distance values
// @return : the recognized digit (0-9)
//

bit4 knn_vote(bit6 knn_set[10][K_CONST]) {
  bit6 sorted_distances[10*K_CONST];
  bit4 sorted_labels[10*K_CONST];

  sort_knn(knn_set, sorted_distances, sorted_labels);

  int votes[10] = {0};
  for (int k = 0; k < K_CONST; ++k) {
    votes[sorted_labels[k]]++;
  }

  int max_votes = 0;
  bit4 max_label = 0;
  for (int i = 0; i < 10; ++i) {
    if (votes[i] > max_votes) {
      max_votes = votes[i];
      max_label = i;
    }
  }
  return max_label;
}