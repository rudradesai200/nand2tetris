//this is a test file created for testing compiler1
// author : Rudra Desai


#include"compiler1.cpp"
#include<gtest/gtest.h>


TEST(Compiler1Tests,classifiertest){
    Parser Q;
    Q.Initialize();
    ASSERT_EQ("identifier",Q.classifier("hello"));
    ASSERT_EQ("keyword",Q.classifier("let"));
    ASSERT_EQ("symbol",Q.classifier("{"));
    ASSERT_NE("symbol",Q.classifier("\""));
}

TEST(Compiler1Tests,RemoveCommentstest){
  Parser Q;
  Q.Initialize();
  ASSERT_EQ(false,Q.RemoveComments("//hello"));
  ASSERT_EQ(false,Q.RemoveComments("/*hello \n hi i am there*/"));
  ASSERT_EQ(true,Q.RemoveComments("/hello"));
}

int main(int argc, char **argv)
    {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    }
