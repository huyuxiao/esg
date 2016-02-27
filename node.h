#ifndef NODE_H_
#define NODE_H_

#include <random>
#include <string>

#include "constants.h"

class Node {
 public:
  Node();
  Node(int state);
  ~Node();

  // Given an existing node, initialize the current object by simulating a one
  // step transition from that node.
  void TransFrom(const Node* node);
  const float GetLogReturn() const;
  const std::string ESGOutput() const;
  const int GetState() const;

 protected:
  virtual const float GetMean() const = 0;
  virtual const float GetSigma() const = 0;
  static const float kMean[kMaxNumStates];
  static const float kSigma[kMaxNumStates];
  static const float kTransProb[kMaxNumStates];
  int state_;

 private:
  // Generates a random number using normal distribution with mean 0.0 and
  // variance 1.0.
  static float GenerateNormalRand();
  // Generates a random number from uniform distribution between 0.0 and 1.0.
  static float GenerateUniformRand();

  static std::random_device random_device_;
  static std::mt19937 random_generator_;
  static std::uniform_real_distribution<float> uniform_;
  static std::normal_distribution<float> normal_;

  float log_return_;
};

class RWNode : public Node {
 public:
  RWNode() {}
  RWNode(int state) : Node(state) {}
  ~RWNode() {}

 protected:
  virtual const float GetMean() const;
  virtual const float GetSigma() const;
};

class RNNode : public Node {
 public:
  RNNode(const float ir);
  ~RNNode() {}

 protected:
  virtual const float GetMean() const;
  virtual const float GetSigma() const;

 private:
  static const float kBSSigma;
  float ir_;
};

#endif  // NODE_H_
