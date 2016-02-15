#include "node.h"

#include <random>
#include <string>

using std::string;

const float Node::kTransProb[] = {0.04f, 0.2f};
const float Node::kMean[] = {0.015f, -0.02f};
const float Node::kSigma[] = {0.035f, 0.08f};


std::random_device Node::random_device_;
std::mt19937 Node::random_generator_(random_device_());
std::uniform_real_distribution<float> Node::uniform_(0, 1);
std::normal_distribution<float> Node::normal_(0, 1);

Node::Node()
  : state_(0),
    log_return_(0.0f) {}

Node::~Node() {}

void Node::TransFrom(const Node* node) {
  int old_state = node->state_;
  log_return_ = node->GetMean() +
	node->GetSigma() * GenerateNormalRand();
  state_ = (old_state + (GenerateUniformRand() <=
			 kTransProb[old_state])) % 2;
}

const string Node::ESGOutput() const {
  return std::to_string(state_) + "," +
    std::to_string(log_return_);
}

// static
float Node::GenerateNormalRand() {
  return normal_(random_generator_);
}

// static
float Node::GenerateUniformRand() {
  return uniform_(random_generator_);
}

const float RWNode::GetMean() const {
  return kMean[state_];
}

const float RWNode::GetSigma() const {
  return kSigma[state_];
}

const float RNNode::kBSSigma =
  sqrt((kTransProb[1] / (kTransProb[1] + kTransProb[2])) * kSigma[2] *
       kSigma[2] + (kTransProb[2] / (kTransProb[1] + kTransProb[2])) *
       kSigma[1] * kSigma[1]);

RNNode::RNNode(const float ir)
  : ir_(ir) {}

const float RNNode::GetMean() const {
  return ir_ - kBSSigma * kBSSigma / 2;
}

const float RNNode::GetSigma() const {
  return kBSSigma;
}
