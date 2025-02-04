// Copyright (c) Facebook, Inc. and its affiliates. (http://www.facebook.com)
#ifndef __STRICTM_FUNCTION_H__
#define __STRICTM_FUNCTION_H__
#include "StrictModules/Objects/instance.h"
#include "StrictModules/Objects/object_type.h"
#include "StrictModules/Objects/signature.h"

#include "StrictModules/analyzer.h"

namespace strictmod::compiler {
class ModuleLoader;
}

namespace strictmod::objects {

class FunctionReturnException {
 public:
  FunctionReturnException(std::shared_ptr<BaseStrictObject> val);

  std::shared_ptr<BaseStrictObject> getVal() {
    return val_;
  }

 private:
  std::shared_ptr<BaseStrictObject> val_;
};

class StrictFunction : public StrictInstance {
 public:
  StrictFunction(
      std::shared_ptr<StrictType> type,
      std::weak_ptr<StrictModuleObject> creator,
      std::string funcName,
      std::string qualName,
      int lineno,
      int col,
      std::vector<stmt_ty> body,
      const EnvT& closure,
      SymtableEntry symbols,
      std::vector<std::string> posonlyArgs,
      std::vector<std::string> posArgs,
      std::vector<std::string> kwonlyArgs,
      std::optional<std::string> varArg,
      std::optional<std::string> kwVarArg,
      std::vector<std::shared_ptr<BaseStrictObject>> posDefaults,
      std::vector<std::shared_ptr<BaseStrictObject>> kwDefaults,
      compiler::ModuleLoader* loader,
      std::string fileName,
      std::shared_ptr<BaseStrictObject> annotations,
      bool useFutureAnnotations = true,
      bool isCoroutine = false);
  // accessors
  bool isCoroutine() const {
    return isCoroutine_;
  }

  bool useFutureAnnotations() const {
    return useFutureAnnotations_;
  }

  SymtableEntry getSymtableEntry() const {
    return symbols_;
  }

  const std::vector<stmt_ty>& getBody() const {
    return body_;
  }

  const std::string& getFuncName() const {
    return funcName_;
  }

  const FuncSignature& getSignature() const {
    return signature_;
  }

  Analyzer getFuncAnalyzer(
      const CallerContext& caller,
      BaseErrorSink* errorSink);

  virtual std::string getDisplayName() const override;

 private:
  std::string funcName_;
  std::string qualName_;
  int lineno_; // line and col at which this function is defined
  int col_;

  std::vector<stmt_ty> body_;
  EnvT closure_;
  SymtableEntry symbols_;

  std::vector<std::string> posonlyArgs_;
  std::vector<std::string> posArgs_;
  std::vector<std::string> kwonlyArgs_;
  std::optional<std::string> varArg_;
  std::optional<std::string> kwVarArg_;
  std::vector<std::shared_ptr<BaseStrictObject>> posDefaults_;
  std::vector<std::shared_ptr<BaseStrictObject>>
      kwDefaults_; // same size as kwonlyArgs_

  compiler::ModuleLoader* loader_;
  std::string fileName_;

  std::shared_ptr<BaseStrictObject> annotations_;

  bool useFutureAnnotations_; // whether function is defined with future
                              // annotations
  bool isCoroutine_;
  FuncSignature signature_;
};

class StrictFuncType : public StrictObjectType {
 public:
  using StrictObjectType::StrictObjectType;
  virtual std::shared_ptr<BaseStrictObject> getDescr(
      std::shared_ptr<BaseStrictObject> obj,
      std::shared_ptr<BaseStrictObject> inst,
      std::shared_ptr<StrictType> type,
      const CallerContext& caller) override;

  virtual std::shared_ptr<BaseStrictObject> call(
      std::shared_ptr<BaseStrictObject> obj,
      const std::vector<std::shared_ptr<BaseStrictObject>>& args,
      const std::vector<std::string>& argNames,
      const CallerContext& caller) override;
};

} // namespace strictmod::objects
#endif // __STRICTM_FUNCTION_H__
