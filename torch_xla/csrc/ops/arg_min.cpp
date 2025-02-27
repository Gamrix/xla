#include "torch_xla/csrc/ops/arg_min.h"

#include "torch_xla/csrc/lowering_context.h"
#include "torch_xla/csrc/ops/infer_output_shape.h"
#include "torch_xla/csrc/reduction.h"

namespace torch_xla {
namespace ir {
namespace ops {
namespace {

xla::Shape NodeOutputShape(const Value& input, int64_t dim, bool keepdim) {
  auto lower_for_shape_fn =
      [&](absl::Span<const xla::XlaOp> operands) -> xla::XlaOp {
    return BuildArgMin(operands[0], dim, keepdim);
  };
  return InferOutputShape({input.xla_shape()}, lower_for_shape_fn);
}

}  // namespace

ArgMin::ArgMin(const Value& input, int64_t dim, bool keepdim)
    : Node(torch::lazy::OpKind(at::aten::argmin), {input},
           [&]() { return NodeOutputShape(input, dim, keepdim); },
           /*num_outputs=*/1, torch::lazy::MHash(dim, keepdim)),
      dim_(dim),
      keepdim_(keepdim) {}

NodePtr ArgMin::Clone(OpList operands) const {
  return ir::MakeNode<ArgMin>(operands.at(0), dim_, keepdim_);
}

XlaOpVector ArgMin::Lower(LoweringContext* loctx) const {
  xla::XlaOp input = loctx->GetOutputOp(operand(0));
  return ReturnOp(BuildArgMin(input, dim_, keepdim_), loctx);
}

std::string ArgMin::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", dim=" << dim_ << ", keepdim=" << keepdim_;
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_xla
