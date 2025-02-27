#include "torch_xla/csrc/ops/rrelu_with_noise.h"

#include "torch_xla/csrc/elementwise.h"
#include "torch_xla/csrc/helpers.h"
#include "torch_xla/csrc/lowering_context.h"
#include "torch_xla/csrc/ops/scalar.h"

namespace torch_xla {
namespace ir {
namespace ops {

RreluWithNoise::RreluWithNoise(const Value& input, const Value& seed,
                               const at::Scalar& lower, const at::Scalar& upper,
                               bool training)
    : Node(torch::lazy::OpKind(at::aten::rrelu_with_noise), {input, seed},
           xla::ShapeUtil::MakeTupleShape(
               {input.xla_shape(), input.xla_shape()}),
           /*num_outputs=*/2,
           torch::lazy::MHash(ScalarHash(lower), ScalarHash(upper), training)),
      lower_(std::move(lower)),
      upper_(std::move(upper)),
      training_(training) {}

NodePtr RreluWithNoise::Clone(OpList operands) const {
  return ir::MakeNode<RreluWithNoise>(operands.at(0), operands.at(1), lower_,
                                      upper_, training_);
}

XlaOpVector RreluWithNoise::Lower(LoweringContext* loctx) const {
  xla::XlaOp input = loctx->GetOutputOp(operand(0));
  xla::XlaOp rng_seed = loctx->GetOutputOp(operand(1));
  return ReturnOps(BuildRrelu(input, lower_, upper_, training_, rng_seed),
                   loctx);
}

std::string RreluWithNoise::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", lower=" << lower_ << ", upper=" << upper_
     << ", training=" << training_;
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_xla
