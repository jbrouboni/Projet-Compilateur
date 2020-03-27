
// Generated from ifcc.g4 by ANTLR 4.7.2

#pragma once

#include <string>
#include <map>
#include "antlr4-runtime.h"
#include "antlr4-generated/ifccBaseVisitor.h"

/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */

class  Visitor : public ifccBaseVisitor {
private:
  int cpt = 0;
  std::map<std::string,int>  map;

public:
  virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override {
	  return visitChildren(ctx);
	}

  virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override {
		std::cout<<".text\n"<<".globl	main"<<std::endl;
		return visit(ctx->decl().at(0));                // A modifier: visiter tous les elements dans le vecteur
  }

  virtual antlrcpp::Any visitDecl(ifccParser::DeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunc_decl(ifccParser::Func_declContext *ctx) override {
		std::cout<<ctx->NAME()->getText()<<":\n"
      " pushq	%rbp\n"
      " movq	%rsp, %rbp"<<std::endl;
      return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVariable_declaration(ifccParser::Variable_declarationContext *ctx) override {
    int size_type = 0;
    if(ctx->type()->getText()=="int"){
      size_type=4;
    }
    for(auto name:ctx->NAME()){
      if(map.count(name->getText())==0)
        map.insert(std::pair<std::string,int>(name->getText(),size_type));
      else{
        std::cout<<"redeclaration of \'"<<name->getText()<<"\' with no linkage"<<std::endl;
      }
    }
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVariable_affectation(ifccParser::Variable_affectationContext *ctx) override {
    std::string name = ctx->NAME()->getText();
    std::string type = ctx->type()->getText();
    if(map.count(name)==0){
      if(type=="int"){
        cpt-=4;                      // A modifier: les autres types comme char, int64
      }
      std::string value = visit(ctx->expr());       // renvoie la valeur d'une constante ou l'adresse d'une variable
      std::string suffixeAdr = "(%rbp)";
       if(value=="%eax"){
        std::cout<<" movl	 %eax"<< ","<<cpt<<"(%rbp)"<<std::endl;
      }else if(value.rfind(suffixeAdr) != (value.length() - suffixeAdr.length())){
        std::cout<< " movl	 $"<< value <<","<<cpt<<"(%rbp)"<<std::endl;
       }else{
        std::cout <<  " movl	"<< value <<", %eax"<< std::endl;
        std::cout<< " movl	 %eax"<< ","<<cpt<<"(%rbp)"<<std::endl;
       }
      map.insert(std::pair<std::string,int>(ctx->NAME()->getText(),cpt));
    }else{
      std::cout<<"redeclaration of  \'"<<name<<"\' with no linkage"<<std::endl;
    }            
    return 0;
  }

  virtual antlrcpp::Any visitType(ifccParser::TypeContext *ctx) override {
		return visitChildren(ctx);
	}

  virtual antlrcpp::Any visitParam_list(ifccParser::Param_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParam(ifccParser::ParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBloc(ifccParser::BlocContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStmt_list(ifccParser::Stmt_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStmt(ifccParser::StmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAff_stmt(ifccParser::Aff_stmtContext *ctx) override {
    std::string name = ctx->NAME()->getText();
    std::string value = visit(ctx->expr());
    if(map.count(name)==1){
      int val = map[name];
      if(val>0){
        cpt-=val;
        map[name] = cpt;
      }
      std::cout<<"  movl  $"<<value<<", "<<map[name]<<"(%rbp)"<<std::endl;
    }else if(map.count(name)==0){
      std::cout<<"\'"<<name<<"\' undeclared (first use in this function)"<<std::endl;
    }else{
      std::cout<<"redeclaration of  \'"<<name<<"\' with no linkage"<<std::endl;
    }
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpr_stmt(ifccParser::Expr_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitArgs(ifccParser::ArgsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitName(ifccParser::NameContext *ctx) override {
    std::string name = ctx->NAME()->getText();
    if(map.count(name)==1){
      return (std::string)(std::to_string(map[name])+"(%rbp)");
    }
    return "error";    // A modifier: traitement d'exception
  }

  virtual antlrcpp::Any visitParenthese(ifccParser::ParentheseContext *ctx) override {
    return visit(ctx->expr());
  }

  virtual antlrcpp::Any visitNombre(ifccParser::NombreContext *ctx) override {
    std::string value = ctx->NOMBRE()->getText();
    return value;
  }

  virtual antlrcpp::Any visitStar(ifccParser::StarContext *ctx) override {
    std::string expr0 = visit(ctx->expr(0));
    std::string expr1 = visit(ctx->expr(1));
    std::string suffixeAdr = "(%rbp)";
    
    if(expr0.rfind(suffixeAdr) == (expr0.length() - suffixeAdr.length())&&expr1.rfind(suffixeAdr) == (expr1.length() - suffixeAdr.length())){
      std::cout<<"  movl  "<<	expr0<<", %edx\n"
               <<" movl  "<<	expr1<<", %eax\n"
               <<"  imull	%edx, %eax"<<std::endl;
    }else if(expr0.rfind(suffixeAdr) == (expr0.length() - suffixeAdr.length())){
      std::cout<<"  movl"<<expr0<<", %eax\n"
      " imull	$"<<expr1<<", %eax"<<std::endl;
    }else if(expr1.rfind(suffixeAdr) == (expr1.length() - suffixeAdr.length())){
      std::cout<<"movl"<<expr1<<", %eax\n"
      " imull	$"<<expr0<<", %eax"<<std::endl;
    }else{
      return std::to_string((int)stoi(expr0)*(int)stoi(expr1));
    }
    return (std::string)"%eax"; 
  }

  virtual antlrcpp::Any visitDiv(ifccParser::DivContext *ctx) override {
    std::string expr0 = visit(ctx->expr(0));
    std::string expr1 = visit(ctx->expr(1));
    std::string suffixeAdr = "(%rbp)";
    
    if(expr0.rfind(suffixeAdr) == (expr0.length() - suffixeAdr.length())&&expr1.rfind(suffixeAdr) == (expr1.length() - suffixeAdr.length())){
      std::cout<<"  movl "<<	expr0<<", %edx\n"
               <<" movl "<<	expr1<<", %eax\n"
               <<"  ctld\n  idivl	%edx, %eax"<<std::endl;
    }else if(expr0.rfind(suffixeAdr) == (expr0.length() - suffixeAdr.length())){
      std::cout<<"  movl "<<expr0<<", %eax\n"
      " idivl	$"<<expr1<<", %eax"<<std::endl;
    }else if(expr1.rfind(suffixeAdr) == (expr1.length() - suffixeAdr.length())){
      std::cout<<"movl "<<expr1<<", %eax\n"
      " idivl	$"<<expr0<<", %eax"<<std::endl;
    }else{
      return std::to_string((int)stoi(expr0)/(int)stoi(expr1));
    }
    return (std::string)"%eax";
  }

  virtual antlrcpp::Any visitPlus(ifccParser::PlusContext *ctx) override {
    std::string expr0 = visit(ctx->expr(0));
    std::string expr1 = visit(ctx->expr(1));
    std::string suffixeAdr = "(%rbp)";

    if(expr0.rfind(suffixeAdr) == (expr0.length() - suffixeAdr.length())&&expr1.rfind(suffixeAdr) == (expr1.length() - suffixeAdr.length())){
      std::cout<<"movl "<<	expr0<<", %edx\n"
        <<" movl  "<<	expr1<<", %eax\n"
        <<"  addl	%edx, %eax"<<std::endl;
    }else if(expr0.rfind(suffixeAdr) == (expr0.length() - suffixeAdr.length())){
      std::cout<<"  movl "<<expr0<<", %eax\n"
      " addl	$"<<expr1<<", %eax"<<std::endl;
    }else if(expr1.rfind(suffixeAdr) == (expr1.length() - suffixeAdr.length())){
      std::cout<<"movl "<<expr1<<", %eax\n"
      " addl	$"<<expr0<<", %eax"<<std::endl;
    }else{
      return std::to_string((int)stoi(expr0)+(int)stoi(expr1));
    }
    return (std::string)"%eax";
  }

  virtual antlrcpp::Any visitMinus(ifccParser::MinusContext *ctx) override {
    std::string expr0 = visit(ctx->expr(0));
    std::string expr1 = visit(ctx->expr(1));
    std::string suffixeAdr = "(%rbp)";

    if(expr0.rfind(suffixeAdr) == (expr0.length() - suffixeAdr.length())&&expr1.rfind(suffixeAdr) == (expr1.length() - suffixeAdr.length())){
      std::cout<<"movl "<<	expr0<<", %eax\n"
        <<"  subl  "<<	expr1<<", %eax"<<std::endl;
    }else if(expr0.rfind(suffixeAdr) == (expr0.length() - suffixeAdr.length())){
      std::cout<<"movl "<<expr0<<", %eax\n"
      " subl	$"<<expr1<<", %eax"<<std::endl;
    }else if(expr1.rfind(suffixeAdr) == (expr1.length() - suffixeAdr.length())){
      std::cout<<"movl  $"<<expr0<<", %eax\n"
      " subl	"<<expr1<<", %eax"<<std::endl;
    }else{
      return std::to_string((int)stoi(expr0)-(int)stoi(expr1));
    }
    return (std::string)"%eax";
  }

  virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override {
    std::string value = visit(ctx->expr());
    std::string suffixeAdr = "(%rbp)";
    if(value=="%eax"){
      std::cout<<" popq	%rbp\n"<<" ret"<<std::endl;
    }else if(value.rfind(suffixeAdr) != (value.length() - suffixeAdr.length())){
      std::cout << " movl $"<<value<<", %eax\n"<<" popq	%rbp\n"<<" ret"<<std::endl;
    }else{
      std::cout << " movl "<<value<<", %eax\n"<<" popq	%rbp\n"<<" ret"<<std::endl;
    }
    return 0;
  }
};

