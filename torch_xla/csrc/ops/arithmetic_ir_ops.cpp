#include "torch_xla/csrc/ops/arithmetic_ir_ops.h"

#include <memory>

#include "torch_xla/csrc/helpers.h"
#include "torch_xla/csrc/lowering_context.h"
#include "torch_xla/csrc/ops/ops.h"

namespace torch_xla {
namespace ir {

NodePtr operator+(const Value& node1, const Value& node2) {
  auto lower_fn = [](const Node& node, LoweringContext* loctx) -> XlaOpVector {
    xla::XlaOp op0 = loctx->GetOutputOp(node.operand(0));
    xla::XlaOp op1 = loctx->GetOutputOp(node.operand(1));
    return node.ReturnOp(XlaHelpers::PromotedAdd(op0, op1), loctx);
  };
  return ops::GenericOp(torch::lazy::OpKind(at::aten::add), {node1, node2},
                        XlaHelpers::GetPromotedBinaryOpShape(node1.xla_shape(),
                                                             node2.xla_shape()),
                        std::move(lower_fn));
}

NodePtr operator-(const Value& node1, const Value& node2) {
  auto lower_fn = [](const Node& node, LoweringContext* loctx) -> XlaOpVector {
    xla::XlaOp op0 = loctx->GetOutputOp(node.operand(0));
    xla::XlaOp op1 = loctx->GetOutputOp(node.operand(1));
    return node.ReturnOp(XlaHelpers::PromotedSub(op0, op1), loctx);
  };
  return ops::GenericOp(torch::lazy::OpKind(at::aten::sub), {node1, node2},
                        XlaHelpers::GetPromotedBinaryOpShape(node1.xla_shape(),
                                                             node2.xla_shape()),
                        std::move(lower_fn));
}

NodePtr operator*(const Value& node1, const Value& node2) {
  auto lower_fn = [](const Node& node, LoweringContext* loctx) -> XlaOpVector {
    xla::XlaOp op0 = loctx->GetOutputOp(node.operand(0));
    xla::XlaOp op1 = loctx->GetOutputOp(node.operand(1));
    return node.ReturnOp(XlaHelpers::PromotedMul(op0, op1), loctx);
  };
  return ops::GenericOp(torch::lazy::OpKind(at::aten::mul), {node1, node2},
                        XlaHelpers::GetPromotedBinaryOpShape(node1.xla_shape(),
                                                             node2.xla_shape()),
                        std::move(lower_fn));
}

NodePtr operator/(const Value& node1, const Value& node2) {
  auto lower_fn = [](const Node& node, LoweringContext* loctx) -> XlaOpVector {
    xla::XlaOp op0 = loctx->GetOutputOp(node.operand(0));
    xla::XlaOp op1 = loctx->GetOutputOp(node.operand(1));
    return node.ReturnOp(XlaHelpers::PromotedDiv(op0, op1), loctx);
  };
  return ops::GenericOp(torch::lazy::OpKind(at::aten::div), {node1, node2},
                        XlaHelpers::GetPromotedBinaryOpShape(node1.xla_shape(),
                                                             node2.xla_shape()),
                        std::move(lower_fn));
}

}  // namespace ir
}  // namespace torch_xla
