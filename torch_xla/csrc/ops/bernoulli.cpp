#include "torch_xla/csrc/ops/bernoulli.h"

#include "torch_xla/csrc/helpers.h"
#include "torch_xla/csrc/lowering_context.h"
#include "torch_xla/csrc/xla_lower_util.h"

namespace torch_xla {
namespace ir {
namespace ops {

Bernoulli::Bernoulli(const Value& probability, const Value& seed,
                     xla::Shape shape)
    : Node(torch::lazy::OpKind(at::aten::bernoulli), {probability, seed},
           std::move(shape)) {}

NodePtr Bernoulli::Clone(OpList operands) const {
  return ir::MakeNode<Bernoulli>(operands.at(0), operands.at(1), xla_shape());
}

XlaOpVector Bernoulli::Lower(LoweringContext* loctx) const {
  xla::XlaOp probability = loctx->GetOutputOp(operand(0));
  xla::XlaOp rng_seed = loctx->GetOutputOp(operand(1));
  const xla::Shape& probability_shape = XlaHelpers::ShapeOfXlaOp(probability);
  xla::Shape bcast_shape(xla_shape());
  bcast_shape.set_element_type(probability_shape.element_type());
  xla::XlaOp bcast_probability = XlaHelpers::ImplicitBroadcast(
      probability, probability_shape, bcast_shape);
  return ReturnOp(
      BuildBernoulli(bcast_probability, rng_seed, xla_shape().element_type()),
      loctx);
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_xla
