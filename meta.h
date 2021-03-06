#pragma once
#ifndef METAS
#include <tuple>

#define RSEQ_N() \
		 119,118,117,116,115,114,113,112,111,110,\
		 109,108,107,106,105,104,103,102,101,100,\
		 99,98,97,96,95,94,93,92,91,90, \
		 89,88,87,86,85,84,83,82,81,80, \
		 79,78,77,76,75,74,73,72,71,70, \
		 69,68,67,66,65,64,63,62,61,60, \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

#define ARG_N( \
         _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
		 _61,_62,_63,_64,_65,_66,_67,_68,_69,_70, \
		 _71,_72,_73,_74,_75,_76,_77,_78,_79,_80, \
		 _81,_82,_83,_84,_85,_86,_87,_88,_89,_90, \
		 _91,_92,_93,_94,_95,_96,_97,_98,_99,_100, \
		 _101,_102,_103,_104,_105,_106,_107,_108,_109,_110, \
         _111,_112,_113,_114,_115,_116,_117,_118,_119,N, ...) N
#define MARCO_EXPAND(...)			__VA_ARGS__
#define GET_ARG_COUNT_INNER(...)    MARCO_EXPAND(ARG_N(__VA_ARGS__))
#define GET_ARG_COUNT(...)          GET_ARG_COUNT_INNER(__VA_ARGS__, RSEQ_N())

#define MACRO_CONCAT0(A, B) A##_##B
#define MACRO_CONCAT(A, B) MACRO_CONCAT0(A, B)

#define PACKAGE(packg) packg
#define MAKE_TUPLE_1(X1) #X1
#define MAKE_TUPLE_2(X1,...) #X1,PACKAGE(MAKE_TUPLE_1(__VA_ARGS__))
#define MAKE_TUPLE_3(X1,...) #X1,PACKAGE(MAKE_TUPLE_2(__VA_ARGS__))
#define MAKE_TUPLE_4(X1,...) #X1,PACKAGE(MAKE_TUPLE_3(__VA_ARGS__))
#define MAKE_TUPLE_5(X1,...) #X1,PACKAGE(MAKE_TUPLE_4(__VA_ARGS__))
#define MAKE_TUPLE_6(X1,...) #X1,PACKAGE(MAKE_TUPLE_5(__VA_ARGS__))
#define MAKE_TUPLE_7(X1,...) #X1,PACKAGE(MAKE_TUPLE_6(__VA_ARGS__))
#define MAKE_TUPLE_8(X1,...) #X1,PACKAGE(MAKE_TUPLE_7(__VA_ARGS__))
#define MAKE_TUPLE_9(X1,...) #X1,PACKAGE(MAKE_TUPLE_8(__VA_ARGS__))
#define MAKE_TUPLE_10(X1,...) #X1,PACKAGE(MAKE_TUPLE_9(__VA_ARGS__))
#define MAKE_TUPLE_11(X1,...) #X1,PACKAGE(MAKE_TUPLE_10(__VA_ARGS__))
#define MAKE_TUPLE_12(X1,...) #X1,PACKAGE(MAKE_TUPLE_11(__VA_ARGS__))
#define MAKE_TUPLE_13(X1,...) #X1,PACKAGE(MAKE_TUPLE_12(__VA_ARGS__))
#define MAKE_TUPLE_14(X1,...) #X1,PACKAGE(MAKE_TUPLE_13(__VA_ARGS__))
#define MAKE_TUPLE_15(X1,...) #X1,PACKAGE(MAKE_TUPLE_14(__VA_ARGS__))
#define MAKE_TUPLE_16(X1,...) #X1,PACKAGE(MAKE_TUPLE_15(__VA_ARGS__))
#define MAKE_TUPLE_17(X1,...) #X1,PACKAGE(MAKE_TUPLE_16(__VA_ARGS__))
#define MAKE_TUPLE_18(X1,...) #X1,PACKAGE(MAKE_TUPLE_17(__VA_ARGS__))
#define MAKE_TUPLE_19(X1,...) #X1,PACKAGE(MAKE_TUPLE_18(__VA_ARGS__))
#define MAKE_TUPLE_20(X1,...) #X1,PACKAGE(MAKE_TUPLE_19(__VA_ARGS__))
#define MAKE_TUPLE_21(X1,...) #X1,PACKAGE(MAKE_TUPLE_20(__VA_ARGS__))
#define MAKE_TUPLE_22(X1,...) #X1,PACKAGE(MAKE_TUPLE_21(__VA_ARGS__))
#define MAKE_TUPLE_23(X1,...) #X1,PACKAGE(MAKE_TUPLE_22(__VA_ARGS__))
#define MAKE_TUPLE_24(X1,...) #X1,PACKAGE(MAKE_TUPLE_23(__VA_ARGS__))
#define MAKE_TUPLE_25(X1,...) #X1,PACKAGE(MAKE_TUPLE_24(__VA_ARGS__))
#define MAKE_TUPLE_26(X1,...) #X1,PACKAGE(MAKE_TUPLE_25(__VA_ARGS__))
#define MAKE_TUPLE_27(X1,...) #X1,PACKAGE(MAKE_TUPLE_26(__VA_ARGS__))
#define MAKE_TUPLE_28(X1,...) #X1,PACKAGE(MAKE_TUPLE_27(__VA_ARGS__))
#define MAKE_TUPLE_29(X1,...) #X1,PACKAGE(MAKE_TUPLE_28(__VA_ARGS__))
#define MAKE_TUPLE_30(X1,...) #X1,PACKAGE(MAKE_TUPLE_29(__VA_ARGS__))
#define MAKE_TUPLE_31(X1,...) #X1,PACKAGE(MAKE_TUPLE_30(__VA_ARGS__))
#define MAKE_TUPLE_32(X1,...) #X1,PACKAGE(MAKE_TUPLE_31(__VA_ARGS__))
#define MAKE_TUPLE_33(X1,...) #X1,PACKAGE(MAKE_TUPLE_32(__VA_ARGS__))
#define MAKE_TUPLE_34(X1,...) #X1,PACKAGE(MAKE_TUPLE_33(__VA_ARGS__))
#define MAKE_TUPLE_35(X1,...) #X1,PACKAGE(MAKE_TUPLE_34(__VA_ARGS__))
#define MAKE_TUPLE(N, ...) std::make_tuple(MACRO_CONCAT(MAKE_TUPLE, N)(__VA_ARGS__))

/*?????????????????????????????????struct??????class??????????????????????????????????????????????????????
?????????????????????????????????????????????,???????????????????????????????????????????????????*/
#define METAS(...) \
public:\
	auto Metas() const->decltype(std::tie(__VA_ARGS__)){return std::tie(__VA_ARGS__);}\
	auto Names() const->decltype(MAKE_TUPLE(GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)){return MAKE_TUPLE(GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__);}\
	auto Metas()->decltype(std::tie(__VA_ARGS__)){return std::tie(__VA_ARGS__);}\
	auto Names()->decltype(MAKE_TUPLE(GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)){return MAKE_TUPLE(GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__);}\
	enum {element_size = GET_ARG_COUNT(__VA_ARGS__)}
#endif
