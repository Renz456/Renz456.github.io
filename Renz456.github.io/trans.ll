; ModuleID = 'trans.c'
source_filename = "trans.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [21 x i8] c"Transpose submission\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"try\00", align 1
@.str.2 = private unnamed_addr constant [16 x i8] c"Basic transpose\00", align 1
@.str.3 = private unnamed_addr constant [36 x i8] c"Transpose using the temporary array\00", align 1

; Function Attrs: nofree norecurse nosync nounwind uwtable
define dso_local void @transpose_1024(i64 noundef %0, i64 noundef %1, double* nocapture noundef readonly %2, double* nocapture noundef writeonly %3, double* nocapture readnone %4) #0 !dbg !9 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !22, metadata !DIExpression()), !dbg !41
  call void @llvm.dbg.value(metadata i64 %1, metadata !23, metadata !DIExpression()), !dbg !41
  call void @llvm.dbg.value(metadata double* %2, metadata !24, metadata !DIExpression()), !dbg !41
  call void @llvm.dbg.value(metadata double* %3, metadata !25, metadata !DIExpression()), !dbg !41
  call void @llvm.dbg.value(metadata double* undef, metadata !26, metadata !DIExpression()), !dbg !41
  call void @llvm.dbg.value(metadata i64 0, metadata !27, metadata !DIExpression()), !dbg !42
  br label %6, !dbg !43

6:                                                ; preds = %31, %5
  %7 = phi i64 [ 0, %5 ], [ %8, %31 ]
  call void @llvm.dbg.value(metadata i64 %7, metadata !27, metadata !DIExpression()), !dbg !42
  %8 = add nuw nsw i64 %7, 8
  call void @llvm.dbg.value(metadata i64 0, metadata !29, metadata !DIExpression()), !dbg !44
  br label %9, !dbg !45

9:                                                ; preds = %12, %6
  %10 = phi i64 [ %11, %12 ], [ 0, %6 ]
  call void @llvm.dbg.value(metadata i64 %10, metadata !29, metadata !DIExpression()), !dbg !44
  call void @llvm.dbg.value(metadata i64 %7, metadata !33, metadata !DIExpression()), !dbg !46
  %11 = add nuw nsw i64 %10, 8
  br label %14, !dbg !47

12:                                               ; preds = %27
  call void @llvm.dbg.value(metadata i64 %11, metadata !29, metadata !DIExpression()), !dbg !44
  %13 = icmp ult i64 %10, 1015, !dbg !48
  br i1 %13, label %9, label %31, !dbg !45, !llvm.loop !49

14:                                               ; preds = %9, %27
  %15 = phi i64 [ %28, %27 ], [ %7, %9 ]
  call void @llvm.dbg.value(metadata i64 %15, metadata !33, metadata !DIExpression()), !dbg !46
  call void @llvm.dbg.value(metadata i64 %10, metadata !37, metadata !DIExpression()), !dbg !52
  %16 = mul nsw i64 %15, %0
  %17 = getelementptr inbounds double, double* %2, i64 %16
  %18 = getelementptr inbounds double, double* %3, i64 %15
  br label %19, !dbg !53

19:                                               ; preds = %19, %14
  %20 = phi i64 [ %10, %14 ], [ %25, %19 ]
  call void @llvm.dbg.value(metadata i64 %20, metadata !37, metadata !DIExpression()), !dbg !52
  %21 = getelementptr inbounds double, double* %17, i64 %20, !dbg !54
  %22 = load double, double* %21, align 8, !dbg !54, !tbaa !57
  %23 = mul nsw i64 %20, %1, !dbg !61
  %24 = getelementptr inbounds double, double* %18, i64 %23, !dbg !61
  store double %22, double* %24, align 8, !dbg !62, !tbaa !57
  %25 = add nuw nsw i64 %20, 1, !dbg !63
  call void @llvm.dbg.value(metadata i64 %25, metadata !37, metadata !DIExpression()), !dbg !52
  %26 = icmp ult i64 %25, %11, !dbg !64
  br i1 %26, label %19, label %27, !dbg !53, !llvm.loop !65

27:                                               ; preds = %19
  %28 = add nuw nsw i64 %15, 1, !dbg !67
  call void @llvm.dbg.value(metadata i64 %28, metadata !33, metadata !DIExpression()), !dbg !46
  %29 = icmp ult i64 %28, %8, !dbg !68
  br i1 %29, label %14, label %12, !dbg !47, !llvm.loop !69

30:                                               ; preds = %31
  ret void, !dbg !71

31:                                               ; preds = %12
  call void @llvm.dbg.value(metadata i64 %8, metadata !27, metadata !DIExpression()), !dbg !42
  %32 = icmp ult i64 %7, 1016, !dbg !72
  br i1 %32, label %6, label %30, !dbg !43, !llvm.loop !73
}

; Function Attrs: nounwind uwtable
define dso_local void @registerFunctions() local_unnamed_addr #1 !dbg !75 {
  tail call void @registerTransFunction(void (i64, i64, double*, double*, double*)* noundef nonnull @transpose_submit, i8* noundef getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i64 0, i64 0)) #4, !dbg !79
  tail call void @registerTransFunction(void (i64, i64, double*, double*, double*)* noundef nonnull @transpose_1024, i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0)) #4, !dbg !80
  tail call void @registerTransFunction(void (i64, i64, double*, double*, double*)* noundef nonnull @trans_basic, i8* noundef getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i64 0, i64 0)) #4, !dbg !81
  tail call void @registerTransFunction(void (i64, i64, double*, double*, double*)* noundef nonnull @trans_tmp, i8* noundef getelementptr inbounds ([36 x i8], [36 x i8]* @.str.3, i64 0, i64 0)) #4, !dbg !82
  ret void, !dbg !83
}

declare !dbg !84 void @registerTransFunction(void (i64, i64, double*, double*, double*)* noundef, i8* noundef) local_unnamed_addr #2

; Function Attrs: nofree norecurse nosync nounwind uwtable
define internal void @transpose_submit(i64 noundef %0, i64 noundef %1, double* nocapture noundef readonly %2, double* nocapture noundef writeonly %3, double* nocapture noundef writeonly %4) #0 !dbg !92 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !94, metadata !DIExpression()), !dbg !116
  call void @llvm.dbg.value(metadata i64 %1, metadata !95, metadata !DIExpression()), !dbg !116
  call void @llvm.dbg.value(metadata double* %2, metadata !96, metadata !DIExpression()), !dbg !116
  call void @llvm.dbg.value(metadata double* %3, metadata !97, metadata !DIExpression()), !dbg !116
  call void @llvm.dbg.value(metadata double* %4, metadata !98, metadata !DIExpression()), !dbg !116
  %6 = icmp eq i64 %0, 32, !dbg !117
  %7 = icmp eq i64 %1, 32
  %8 = and i1 %6, %7, !dbg !118
  br i1 %8, label %13, label %9, !dbg !118

9:                                                ; preds = %5
  %10 = icmp eq i64 %0, 1024, !dbg !119
  %11 = icmp eq i64 %1, 1024
  %12 = and i1 %10, %11, !dbg !120
  br i1 %12, label %13, label %87, !dbg !120

13:                                               ; preds = %9, %5
  br label %14, !dbg !121

14:                                               ; preds = %13, %22
  %15 = phi i64 [ %16, %22 ], [ 0, %13 ]
  call void @llvm.dbg.value(metadata i64 %15, metadata !102, metadata !DIExpression()), !dbg !122
  call void @llvm.dbg.value(metadata i64 0, metadata !104, metadata !DIExpression()), !dbg !123
  %16 = add i64 %15, 8
  %17 = icmp eq i64 %15, -8
  br i1 %17, label %18, label %24, !dbg !124

18:                                               ; preds = %14, %18
  %19 = phi i64 [ %20, %18 ], [ 0, %14 ]
  call void @llvm.dbg.value(metadata i64 %19, metadata !104, metadata !DIExpression()), !dbg !123
  call void @llvm.dbg.value(metadata i64 %15, metadata !108, metadata !DIExpression()), !dbg !125
  %20 = add i64 %19, 8, !dbg !126
  call void @llvm.dbg.value(metadata i64 %20, metadata !104, metadata !DIExpression()), !dbg !123
  %21 = icmp ult i64 %20, %0, !dbg !127
  br i1 %21, label %18, label %22, !dbg !128, !llvm.loop !129

22:                                               ; preds = %30, %18
  call void @llvm.dbg.value(metadata i64 %16, metadata !102, metadata !DIExpression()), !dbg !122
  %23 = icmp ult i64 %16, %1, !dbg !131
  br i1 %23, label %14, label %132, !dbg !121, !llvm.loop !132

24:                                               ; preds = %14, %30
  %25 = phi i64 [ %26, %30 ], [ 0, %14 ]
  call void @llvm.dbg.value(metadata i64 %25, metadata !104, metadata !DIExpression()), !dbg !123
  call void @llvm.dbg.value(metadata i64 %15, metadata !108, metadata !DIExpression()), !dbg !125
  %26 = add i64 %25, 8
  %27 = icmp eq i64 %25, -8
  %28 = icmp eq i64 %15, %25
  br i1 %27, label %29, label %32, !dbg !134

29:                                               ; preds = %24
  br i1 %28, label %52, label %30, !dbg !135

30:                                               ; preds = %49, %79, %52, %29
  call void @llvm.dbg.value(metadata i64 %26, metadata !104, metadata !DIExpression()), !dbg !123
  %31 = icmp ult i64 %26, %0, !dbg !127
  br i1 %31, label %24, label %22, !dbg !128, !llvm.loop !129

32:                                               ; preds = %24
  br i1 %28, label %63, label %33, !dbg !135

33:                                               ; preds = %32, %49
  %34 = phi i64 [ %50, %49 ], [ %15, %32 ]
  call void @llvm.dbg.value(metadata i64 %34, metadata !108, metadata !DIExpression()), !dbg !125
  call void @llvm.dbg.value(metadata i64 %25, metadata !112, metadata !DIExpression()), !dbg !136
  %35 = mul nsw i64 %34, %0
  %36 = getelementptr inbounds double, double* %2, i64 %35
  %37 = getelementptr inbounds double, double* %3, i64 %34
  br label %38, !dbg !134

38:                                               ; preds = %46, %33
  %39 = phi i64 [ %25, %33 ], [ %47, %46 ]
  call void @llvm.dbg.value(metadata i64 %39, metadata !112, metadata !DIExpression()), !dbg !136
  %40 = icmp eq i64 %34, %39, !dbg !137
  br i1 %40, label %46, label %41, !dbg !141

41:                                               ; preds = %38
  %42 = getelementptr inbounds double, double* %36, i64 %39, !dbg !142
  %43 = load double, double* %42, align 8, !dbg !142, !tbaa !57
  %44 = mul nsw i64 %39, %1, !dbg !143
  %45 = getelementptr inbounds double, double* %37, i64 %44, !dbg !143
  store double %43, double* %45, align 8, !dbg !144, !tbaa !57
  br label %46, !dbg !143

46:                                               ; preds = %41, %38
  %47 = add nuw i64 %39, 1, !dbg !145
  call void @llvm.dbg.value(metadata i64 %47, metadata !112, metadata !DIExpression()), !dbg !136
  %48 = icmp ult i64 %47, %26, !dbg !146
  br i1 %48, label %38, label %49, !dbg !134, !llvm.loop !147

49:                                               ; preds = %46
  %50 = add nuw i64 %34, 1, !dbg !149
  call void @llvm.dbg.value(metadata i64 %50, metadata !108, metadata !DIExpression()), !dbg !125
  %51 = icmp ult i64 %50, %16, !dbg !150
  br i1 %51, label %33, label %30, !dbg !124, !llvm.loop !151

52:                                               ; preds = %29, %52
  %53 = phi i64 [ %61, %52 ], [ %15, %29 ]
  call void @llvm.dbg.value(metadata i64 %53, metadata !108, metadata !DIExpression()), !dbg !125
  call void @llvm.dbg.value(metadata i64 %25, metadata !112, metadata !DIExpression()), !dbg !136
  %54 = mul nsw i64 %53, %0, !dbg !153
  %55 = getelementptr inbounds double, double* %2, i64 %54, !dbg !153
  %56 = getelementptr inbounds double, double* %55, i64 %53, !dbg !153
  %57 = load double, double* %56, align 8, !dbg !153, !tbaa !57
  %58 = mul nsw i64 %53, %1, !dbg !155
  %59 = getelementptr inbounds double, double* %3, i64 %58, !dbg !155
  %60 = getelementptr inbounds double, double* %59, i64 %53, !dbg !155
  store double %57, double* %60, align 8, !dbg !156, !tbaa !57
  %61 = add nuw i64 %53, 1, !dbg !149
  call void @llvm.dbg.value(metadata i64 %61, metadata !108, metadata !DIExpression()), !dbg !125
  %62 = icmp ult i64 %61, %16, !dbg !150
  br i1 %62, label %52, label %30, !dbg !124, !llvm.loop !151

63:                                               ; preds = %32, %79
  %64 = phi i64 [ %85, %79 ], [ %15, %32 ]
  call void @llvm.dbg.value(metadata i64 %64, metadata !108, metadata !DIExpression()), !dbg !125
  call void @llvm.dbg.value(metadata i64 %25, metadata !112, metadata !DIExpression()), !dbg !136
  %65 = mul nsw i64 %64, %0
  %66 = getelementptr inbounds double, double* %2, i64 %65
  %67 = getelementptr inbounds double, double* %3, i64 %64
  br label %68, !dbg !134

68:                                               ; preds = %76, %63
  %69 = phi i64 [ %15, %63 ], [ %77, %76 ]
  call void @llvm.dbg.value(metadata i64 %69, metadata !112, metadata !DIExpression()), !dbg !136
  %70 = icmp eq i64 %64, %69, !dbg !137
  br i1 %70, label %76, label %71, !dbg !141

71:                                               ; preds = %68
  %72 = getelementptr inbounds double, double* %66, i64 %69, !dbg !142
  %73 = load double, double* %72, align 8, !dbg !142, !tbaa !57
  %74 = mul nsw i64 %69, %1, !dbg !143
  %75 = getelementptr inbounds double, double* %67, i64 %74, !dbg !143
  store double %73, double* %75, align 8, !dbg !144, !tbaa !57
  br label %76, !dbg !143

76:                                               ; preds = %71, %68
  %77 = add nuw i64 %69, 1, !dbg !145
  call void @llvm.dbg.value(metadata i64 %77, metadata !112, metadata !DIExpression()), !dbg !136
  %78 = icmp ult i64 %77, %26, !dbg !146
  br i1 %78, label %68, label %79, !dbg !134, !llvm.loop !147

79:                                               ; preds = %76
  %80 = getelementptr inbounds double, double* %66, i64 %64, !dbg !153
  %81 = load double, double* %80, align 8, !dbg !153, !tbaa !57
  %82 = mul nsw i64 %64, %1, !dbg !155
  %83 = getelementptr inbounds double, double* %3, i64 %82, !dbg !155
  %84 = getelementptr inbounds double, double* %83, i64 %64, !dbg !155
  store double %81, double* %84, align 8, !dbg !156, !tbaa !57
  %85 = add nuw i64 %64, 1, !dbg !149
  call void @llvm.dbg.value(metadata i64 %85, metadata !108, metadata !DIExpression()), !dbg !125
  %86 = icmp ult i64 %85, %16, !dbg !150
  br i1 %86, label %63, label %30, !dbg !124, !llvm.loop !151

87:                                               ; preds = %9
  %88 = icmp eq i64 %0, %1, !dbg !157
  br i1 %88, label %89, label %107, !dbg !159

89:                                               ; preds = %87
  call void @llvm.dbg.value(metadata i64 %0, metadata !160, metadata !DIExpression()), !dbg !173
  call void @llvm.dbg.value(metadata i64 %1, metadata !163, metadata !DIExpression()), !dbg !173
  call void @llvm.dbg.value(metadata double* %2, metadata !164, metadata !DIExpression()), !dbg !173
  call void @llvm.dbg.value(metadata double* %3, metadata !165, metadata !DIExpression()), !dbg !173
  call void @llvm.dbg.value(metadata double* %4, metadata !166, metadata !DIExpression()), !dbg !173
  call void @llvm.dbg.value(metadata i64 0, metadata !167, metadata !DIExpression()), !dbg !175
  %90 = icmp eq i64 %0, 0, !dbg !176
  br i1 %90, label %132, label %91, !dbg !177

91:                                               ; preds = %89, %104
  %92 = phi i64 [ %105, %104 ], [ 0, %89 ]
  call void @llvm.dbg.value(metadata i64 %92, metadata !167, metadata !DIExpression()), !dbg !175
  call void @llvm.dbg.value(metadata i64 0, metadata !169, metadata !DIExpression()), !dbg !178
  %93 = mul nsw i64 %92, %0
  %94 = getelementptr inbounds double, double* %2, i64 %93
  %95 = getelementptr inbounds double, double* %3, i64 %92
  br label %96, !dbg !179

96:                                               ; preds = %96, %91
  %97 = phi i64 [ 0, %91 ], [ %102, %96 ]
  call void @llvm.dbg.value(metadata i64 %97, metadata !169, metadata !DIExpression()), !dbg !178
  %98 = getelementptr inbounds double, double* %94, i64 %97, !dbg !180
  %99 = load double, double* %98, align 8, !dbg !180, !tbaa !57
  %100 = mul nsw i64 %97, %0, !dbg !183
  %101 = getelementptr inbounds double, double* %95, i64 %100, !dbg !183
  store double %99, double* %101, align 8, !dbg !184, !tbaa !57
  %102 = add nuw i64 %97, 1, !dbg !185
  call void @llvm.dbg.value(metadata i64 %102, metadata !169, metadata !DIExpression()), !dbg !178
  %103 = icmp eq i64 %102, %0, !dbg !186
  br i1 %103, label %104, label %96, !dbg !179, !llvm.loop !187

104:                                              ; preds = %96
  %105 = add nuw i64 %92, 1, !dbg !189
  call void @llvm.dbg.value(metadata i64 %105, metadata !167, metadata !DIExpression()), !dbg !175
  %106 = icmp eq i64 %105, %0, !dbg !176
  br i1 %106, label %132, label %91, !dbg !177, !llvm.loop !190

107:                                              ; preds = %87
  call void @llvm.dbg.value(metadata i64 %0, metadata !192, metadata !DIExpression()), !dbg !209
  call void @llvm.dbg.value(metadata i64 %1, metadata !195, metadata !DIExpression()), !dbg !209
  call void @llvm.dbg.value(metadata double* %2, metadata !196, metadata !DIExpression()), !dbg !209
  call void @llvm.dbg.value(metadata double* %3, metadata !197, metadata !DIExpression()), !dbg !209
  call void @llvm.dbg.value(metadata double* %4, metadata !198, metadata !DIExpression()), !dbg !209
  call void @llvm.dbg.value(metadata i64 0, metadata !199, metadata !DIExpression()), !dbg !211
  %108 = icmp eq i64 %1, 0, !dbg !212
  %109 = icmp eq i64 %0, 0
  %110 = or i1 %109, %108, !dbg !213
  br i1 %110, label %132, label %111, !dbg !213

111:                                              ; preds = %107, %129
  %112 = phi i64 [ %130, %129 ], [ 0, %107 ]
  call void @llvm.dbg.value(metadata i64 %112, metadata !199, metadata !DIExpression()), !dbg !211
  call void @llvm.dbg.value(metadata i64 0, metadata !201, metadata !DIExpression()), !dbg !214
  %113 = mul nsw i64 %112, %0
  %114 = getelementptr inbounds double, double* %2, i64 %113
  %115 = shl i64 %112, 1
  %116 = and i64 %115, 2
  %117 = getelementptr inbounds double, double* %3, i64 %112
  br label %118, !dbg !215

118:                                              ; preds = %118, %111
  %119 = phi i64 [ 0, %111 ], [ %127, %118 ]
  call void @llvm.dbg.value(metadata i64 %119, metadata !201, metadata !DIExpression()), !dbg !214
  call void @llvm.dbg.value(metadata i64 %112, metadata !205, metadata !DIExpression(DW_OP_constu, 1, DW_OP_and, DW_OP_stack_value)), !dbg !216
  %120 = and i64 %119, 1, !dbg !217
  call void @llvm.dbg.value(metadata i64 %120, metadata !208, metadata !DIExpression()), !dbg !216
  %121 = getelementptr inbounds double, double* %114, i64 %119, !dbg !218
  %122 = load double, double* %121, align 8, !dbg !218, !tbaa !57
  %123 = or i64 %120, %116, !dbg !219
  %124 = getelementptr inbounds double, double* %4, i64 %123, !dbg !220
  store double %122, double* %124, align 8, !dbg !221, !tbaa !57
  %125 = mul nsw i64 %119, %1, !dbg !222
  %126 = getelementptr inbounds double, double* %117, i64 %125, !dbg !222
  store double %122, double* %126, align 8, !dbg !223, !tbaa !57
  %127 = add nuw i64 %119, 1, !dbg !224
  call void @llvm.dbg.value(metadata i64 %127, metadata !201, metadata !DIExpression()), !dbg !214
  %128 = icmp eq i64 %127, %0, !dbg !225
  br i1 %128, label %129, label %118, !dbg !215, !llvm.loop !226

129:                                              ; preds = %118
  %130 = add nuw i64 %112, 1, !dbg !228
  call void @llvm.dbg.value(metadata i64 %130, metadata !199, metadata !DIExpression()), !dbg !211
  %131 = icmp eq i64 %130, %1, !dbg !212
  br i1 %131, label %132, label %111, !dbg !213, !llvm.loop !229

132:                                              ; preds = %129, %104, %22, %107, %89
  ret void, !dbg !231
}

; Function Attrs: nofree norecurse nosync nounwind uwtable
define internal void @trans_basic(i64 noundef %0, i64 noundef %1, double* nocapture noundef readonly %2, double* nocapture noundef writeonly %3, double* nocapture noundef readnone %4) #0 !dbg !161 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !160, metadata !DIExpression()), !dbg !232
  call void @llvm.dbg.value(metadata i64 %1, metadata !163, metadata !DIExpression()), !dbg !232
  call void @llvm.dbg.value(metadata double* %2, metadata !164, metadata !DIExpression()), !dbg !232
  call void @llvm.dbg.value(metadata double* %3, metadata !165, metadata !DIExpression()), !dbg !232
  call void @llvm.dbg.value(metadata double* %4, metadata !166, metadata !DIExpression()), !dbg !232
  call void @llvm.dbg.value(metadata i64 0, metadata !167, metadata !DIExpression()), !dbg !233
  %6 = icmp eq i64 %1, 0, !dbg !234
  %7 = icmp eq i64 %0, 0
  %8 = or i1 %6, %7, !dbg !235
  br i1 %8, label %55, label %9, !dbg !235

9:                                                ; preds = %5
  %10 = icmp ugt i64 %0, 1
  %11 = icmp eq i64 %1, 1
  %12 = and i1 %10, %11
  %13 = and i64 %0, -2
  %14 = icmp eq i64 %13, %0
  br label %15, !dbg !235

15:                                               ; preds = %9, %52
  %16 = phi i64 [ %53, %52 ], [ 0, %9 ]
  call void @llvm.dbg.value(metadata i64 %16, metadata !167, metadata !DIExpression()), !dbg !233
  call void @llvm.dbg.value(metadata i64 0, metadata !169, metadata !DIExpression()), !dbg !236
  %17 = mul i64 %16, %0
  %18 = mul nsw i64 %16, %0
  %19 = getelementptr inbounds double, double* %2, i64 %18
  %20 = getelementptr inbounds double, double* %3, i64 %16
  br i1 %12, label %21, label %42, !dbg !237

21:                                               ; preds = %15
  %22 = add i64 %17, %0
  %23 = getelementptr double, double* %2, i64 %22
  %24 = getelementptr double, double* %2, i64 %17
  %25 = add i64 %16, %0
  %26 = getelementptr double, double* %3, i64 %25
  %27 = getelementptr double, double* %3, i64 %16
  %28 = icmp ult double* %27, %23, !dbg !237
  %29 = icmp ult double* %24, %26, !dbg !237
  %30 = and i1 %28, %29, !dbg !237
  br i1 %30, label %42, label %31

31:                                               ; preds = %21, %31
  %32 = phi i64 [ %39, %31 ], [ 0, %21 ], !dbg !238
  %33 = getelementptr inbounds double, double* %19, i64 %32, !dbg !238
  %34 = bitcast double* %33 to <2 x double>*, !dbg !239
  %35 = load <2 x double>, <2 x double>* %34, align 8, !dbg !239, !tbaa !57, !alias.scope !240
  %36 = mul nsw i64 %32, %1, !dbg !238
  %37 = getelementptr inbounds double, double* %20, i64 %36, !dbg !238
  %38 = bitcast double* %37 to <2 x double>*, !dbg !243
  store <2 x double> %35, <2 x double>* %38, align 8, !dbg !243, !tbaa !57, !alias.scope !244, !noalias !240
  %39 = add nuw i64 %32, 2, !dbg !238
  %40 = icmp eq i64 %39, %13, !dbg !238
  br i1 %40, label %41, label %31, !dbg !238, !llvm.loop !246

41:                                               ; preds = %31
  br i1 %14, label %52, label %42, !dbg !237

42:                                               ; preds = %21, %15, %41
  %43 = phi i64 [ 0, %21 ], [ 0, %15 ], [ %13, %41 ]
  br label %44, !dbg !237

44:                                               ; preds = %42, %44
  %45 = phi i64 [ %50, %44 ], [ %43, %42 ]
  call void @llvm.dbg.value(metadata i64 %45, metadata !169, metadata !DIExpression()), !dbg !236
  %46 = getelementptr inbounds double, double* %19, i64 %45, !dbg !239
  %47 = load double, double* %46, align 8, !dbg !239, !tbaa !57
  %48 = mul nsw i64 %45, %1, !dbg !249
  %49 = getelementptr inbounds double, double* %20, i64 %48, !dbg !249
  store double %47, double* %49, align 8, !dbg !243, !tbaa !57
  %50 = add nuw i64 %45, 1, !dbg !238
  call void @llvm.dbg.value(metadata i64 %50, metadata !169, metadata !DIExpression()), !dbg !236
  %51 = icmp eq i64 %50, %0, !dbg !250
  br i1 %51, label %52, label %44, !dbg !237, !llvm.loop !251

52:                                               ; preds = %44, %41
  %53 = add nuw i64 %16, 1, !dbg !252
  call void @llvm.dbg.value(metadata i64 %53, metadata !167, metadata !DIExpression()), !dbg !233
  %54 = icmp eq i64 %53, %1, !dbg !234
  br i1 %54, label %55, label %15, !dbg !235, !llvm.loop !253

55:                                               ; preds = %52, %5
  ret void, !dbg !255
}

; Function Attrs: nofree norecurse nosync nounwind uwtable
define internal void @trans_tmp(i64 noundef %0, i64 noundef %1, double* nocapture noundef readonly %2, double* nocapture noundef writeonly %3, double* nocapture noundef writeonly %4) #0 !dbg !193 {
  call void @llvm.dbg.value(metadata i64 %0, metadata !192, metadata !DIExpression()), !dbg !256
  call void @llvm.dbg.value(metadata i64 %1, metadata !195, metadata !DIExpression()), !dbg !256
  call void @llvm.dbg.value(metadata double* %2, metadata !196, metadata !DIExpression()), !dbg !256
  call void @llvm.dbg.value(metadata double* %3, metadata !197, metadata !DIExpression()), !dbg !256
  call void @llvm.dbg.value(metadata double* %4, metadata !198, metadata !DIExpression()), !dbg !256
  call void @llvm.dbg.value(metadata i64 0, metadata !199, metadata !DIExpression()), !dbg !257
  %6 = icmp eq i64 %1, 0, !dbg !258
  %7 = icmp eq i64 %0, 0
  %8 = or i1 %6, %7, !dbg !259
  br i1 %8, label %30, label %9, !dbg !259

9:                                                ; preds = %5, %27
  %10 = phi i64 [ %28, %27 ], [ 0, %5 ]
  call void @llvm.dbg.value(metadata i64 %10, metadata !199, metadata !DIExpression()), !dbg !257
  call void @llvm.dbg.value(metadata i64 0, metadata !201, metadata !DIExpression()), !dbg !260
  %11 = mul nsw i64 %10, %0
  %12 = getelementptr inbounds double, double* %2, i64 %11
  %13 = shl i64 %10, 1
  %14 = and i64 %13, 2
  %15 = getelementptr inbounds double, double* %3, i64 %10
  br label %16, !dbg !261

16:                                               ; preds = %9, %16
  %17 = phi i64 [ 0, %9 ], [ %25, %16 ]
  call void @llvm.dbg.value(metadata i64 %17, metadata !201, metadata !DIExpression()), !dbg !260
  call void @llvm.dbg.value(metadata i64 %10, metadata !205, metadata !DIExpression(DW_OP_constu, 1, DW_OP_and, DW_OP_stack_value)), !dbg !262
  %18 = and i64 %17, 1, !dbg !263
  call void @llvm.dbg.value(metadata i64 %18, metadata !208, metadata !DIExpression()), !dbg !262
  %19 = getelementptr inbounds double, double* %12, i64 %17, !dbg !264
  %20 = load double, double* %19, align 8, !dbg !264, !tbaa !57
  %21 = or i64 %18, %14, !dbg !265
  %22 = getelementptr inbounds double, double* %4, i64 %21, !dbg !266
  store double %20, double* %22, align 8, !dbg !267, !tbaa !57
  %23 = mul nsw i64 %17, %1, !dbg !268
  %24 = getelementptr inbounds double, double* %15, i64 %23, !dbg !268
  store double %20, double* %24, align 8, !dbg !269, !tbaa !57
  %25 = add nuw i64 %17, 1, !dbg !270
  call void @llvm.dbg.value(metadata i64 %25, metadata !201, metadata !DIExpression()), !dbg !260
  %26 = icmp eq i64 %25, %0, !dbg !271
  br i1 %26, label %27, label %16, !dbg !261, !llvm.loop !272

27:                                               ; preds = %16
  %28 = add nuw i64 %10, 1, !dbg !274
  call void @llvm.dbg.value(metadata i64 %28, metadata !199, metadata !DIExpression()), !dbg !257
  %29 = icmp eq i64 %28, %1, !dbg !258
  br i1 %29, label %30, label %9, !dbg !259, !llvm.loop !275

30:                                               ; preds = %27, %5
  ret void, !dbg !277
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.value(metadata, metadata, metadata) #3

attributes #0 = { nofree norecurse nosync nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7}
!llvm.ident = !{!8}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "trans.c", directory: "/afs/andrew.cmu.edu/usr20/rravanan/private/15418/Renz456.github.io", checksumkind: CSK_MD5, checksum: "8ae7045bcd6d0866281d92d424576c85")
!2 = !{i32 7, !"Dwarf Version", i32 5}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 7, !"PIC Level", i32 2}
!6 = !{i32 7, !"PIE Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 1}
!8 = !{!"Ubuntu clang version 14.0.0-1ubuntu1"}
!9 = distinct !DISubprogram(name: "transpose_1024", scope: !1, file: !1, line: 175, type: !10, scopeLine: 176, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !21)
!10 = !DISubroutineType(types: !11)
!11 = !{null, !12, !12, !15, !15, !20}
!12 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !13, line: 46, baseType: !14)
!13 = !DIFile(filename: "/usr/lib/llvm-14/lib/clang/14.0.0/include/stddef.h", directory: "", checksumkind: CSK_MD5, checksum: "2499dd2361b915724b073282bea3a7bc")
!14 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!15 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !16, size: 64)
!16 = !DICompositeType(tag: DW_TAG_array_type, baseType: !17, elements: !18)
!17 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!18 = !{!19}
!19 = !DISubrange(count: -1)
!20 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !17, size: 64)
!21 = !{!22, !23, !24, !25, !26, !27, !29, !33, !37}
!22 = !DILocalVariable(name: "M", arg: 1, scope: !9, file: !1, line: 175, type: !12)
!23 = !DILocalVariable(name: "N", arg: 2, scope: !9, file: !1, line: 175, type: !12)
!24 = !DILocalVariable(name: "A", arg: 3, scope: !9, file: !1, line: 175, type: !15)
!25 = !DILocalVariable(name: "B", arg: 4, scope: !9, file: !1, line: 175, type: !15)
!26 = !DILocalVariable(name: "tmp", arg: 5, scope: !9, file: !1, line: 176, type: !20)
!27 = !DILocalVariable(name: "i", scope: !28, file: !1, line: 179, type: !12)
!28 = distinct !DILexicalBlock(scope: !9, file: !1, line: 179, column: 5)
!29 = !DILocalVariable(name: "j", scope: !30, file: !1, line: 180, type: !12)
!30 = distinct !DILexicalBlock(scope: !31, file: !1, line: 180, column: 9)
!31 = distinct !DILexicalBlock(scope: !32, file: !1, line: 179, column: 42)
!32 = distinct !DILexicalBlock(scope: !28, file: !1, line: 179, column: 5)
!33 = !DILocalVariable(name: "row", scope: !34, file: !1, line: 181, type: !12)
!34 = distinct !DILexicalBlock(scope: !35, file: !1, line: 181, column: 13)
!35 = distinct !DILexicalBlock(scope: !36, file: !1, line: 180, column: 46)
!36 = distinct !DILexicalBlock(scope: !30, file: !1, line: 180, column: 9)
!37 = !DILocalVariable(name: "col", scope: !38, file: !1, line: 182, type: !12)
!38 = distinct !DILexicalBlock(scope: !39, file: !1, line: 182, column: 17)
!39 = distinct !DILexicalBlock(scope: !40, file: !1, line: 181, column: 54)
!40 = distinct !DILexicalBlock(scope: !34, file: !1, line: 181, column: 13)
!41 = !DILocation(line: 0, scope: !9)
!42 = !DILocation(line: 0, scope: !28)
!43 = !DILocation(line: 179, column: 5, scope: !28)
!44 = !DILocation(line: 0, scope: !30)
!45 = !DILocation(line: 180, column: 9, scope: !30)
!46 = !DILocation(line: 0, scope: !34)
!47 = !DILocation(line: 181, column: 13, scope: !34)
!48 = !DILocation(line: 180, column: 30, scope: !36)
!49 = distinct !{!49, !45, !50, !51}
!50 = !DILocation(line: 186, column: 9, scope: !30)
!51 = !{!"llvm.loop.unroll.disable"}
!52 = !DILocation(line: 0, scope: !38)
!53 = !DILocation(line: 182, column: 17, scope: !38)
!54 = !DILocation(line: 183, column: 35, scope: !55)
!55 = distinct !DILexicalBlock(scope: !56, file: !1, line: 182, column: 58)
!56 = distinct !DILexicalBlock(scope: !38, file: !1, line: 182, column: 17)
!57 = !{!58, !58, i64 0}
!58 = !{!"double", !59, i64 0}
!59 = !{!"omnipotent char", !60, i64 0}
!60 = !{!"Simple C/C++ TBAA"}
!61 = !DILocation(line: 183, column: 21, scope: !55)
!62 = !DILocation(line: 183, column: 33, scope: !55)
!63 = !DILocation(line: 182, column: 54, scope: !56)
!64 = !DILocation(line: 182, column: 42, scope: !56)
!65 = distinct !{!65, !53, !66, !51}
!66 = !DILocation(line: 184, column: 17, scope: !38)
!67 = !DILocation(line: 181, column: 50, scope: !40)
!68 = !DILocation(line: 181, column: 38, scope: !40)
!69 = distinct !{!69, !47, !70, !51}
!70 = !DILocation(line: 185, column: 13, scope: !34)
!71 = !DILocation(line: 301, column: 1, scope: !9)
!72 = !DILocation(line: 179, column: 26, scope: !32)
!73 = distinct !{!73, !43, !74, !51}
!74 = !DILocation(line: 187, column: 5, scope: !28)
!75 = distinct !DISubprogram(name: "registerFunctions", scope: !1, file: !1, line: 309, type: !76, scopeLine: 309, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !78)
!76 = !DISubroutineType(types: !77)
!77 = !{null}
!78 = !{}
!79 = !DILocation(line: 311, column: 5, scope: !75)
!80 = !DILocation(line: 313, column: 5, scope: !75)
!81 = !DILocation(line: 315, column: 5, scope: !75)
!82 = !DILocation(line: 316, column: 5, scope: !75)
!83 = !DILocation(line: 317, column: 1, scope: !75)
!84 = !DISubprogram(name: "registerTransFunction", scope: !85, file: !85, line: 178, type: !86, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !78)
!85 = !DIFile(filename: "./cachelab.h", directory: "/afs/andrew.cmu.edu/usr20/rravanan/private/15418/Renz456.github.io", checksumkind: CSK_MD5, checksum: "e7d7dfb0f94200658b55356a1afed378")
!86 = !DISubroutineType(types: !87)
!87 = !{null, !88, !89}
!88 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !10, size: 64)
!89 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !90, size: 64)
!90 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !91)
!91 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!92 = distinct !DISubprogram(name: "transpose_submit", scope: !1, file: !1, line: 127, type: !10, scopeLine: 128, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !93)
!93 = !{!94, !95, !96, !97, !98, !99, !102, !104, !108, !112}
!94 = !DILocalVariable(name: "M", arg: 1, scope: !92, file: !1, line: 127, type: !12)
!95 = !DILocalVariable(name: "N", arg: 2, scope: !92, file: !1, line: 127, type: !12)
!96 = !DILocalVariable(name: "A", arg: 3, scope: !92, file: !1, line: 127, type: !15)
!97 = !DILocalVariable(name: "B", arg: 4, scope: !92, file: !1, line: 127, type: !15)
!98 = !DILocalVariable(name: "tmp", arg: 5, scope: !92, file: !1, line: 128, type: !20)
!99 = !DILocalVariable(name: "block_size", scope: !100, file: !1, line: 131, type: !12)
!100 = distinct !DILexicalBlock(scope: !101, file: !1, line: 130, column: 59)
!101 = distinct !DILexicalBlock(scope: !92, file: !1, line: 130, column: 9)
!102 = !DILocalVariable(name: "i", scope: !103, file: !1, line: 133, type: !12)
!103 = distinct !DILexicalBlock(scope: !100, file: !1, line: 133, column: 9)
!104 = !DILocalVariable(name: "j", scope: !105, file: !1, line: 135, type: !12)
!105 = distinct !DILexicalBlock(scope: !106, file: !1, line: 135, column: 13)
!106 = distinct !DILexicalBlock(scope: !107, file: !1, line: 133, column: 52)
!107 = distinct !DILexicalBlock(scope: !103, file: !1, line: 133, column: 9)
!108 = !DILocalVariable(name: "row", scope: !109, file: !1, line: 137, type: !12)
!109 = distinct !DILexicalBlock(scope: !110, file: !1, line: 137, column: 17)
!110 = distinct !DILexicalBlock(scope: !111, file: !1, line: 135, column: 56)
!111 = distinct !DILexicalBlock(scope: !105, file: !1, line: 135, column: 13)
!112 = !DILocalVariable(name: "col", scope: !113, file: !1, line: 139, type: !12)
!113 = distinct !DILexicalBlock(scope: !114, file: !1, line: 139, column: 21)
!114 = distinct !DILexicalBlock(scope: !115, file: !1, line: 137, column: 67)
!115 = distinct !DILexicalBlock(scope: !109, file: !1, line: 137, column: 17)
!116 = !DILocation(line: 0, scope: !92)
!117 = !DILocation(line: 130, column: 12, scope: !101)
!118 = !DILocation(line: 130, column: 18, scope: !101)
!119 = !DILocation(line: 130, column: 36, scope: !101)
!120 = !DILocation(line: 130, column: 44, scope: !101)
!121 = !DILocation(line: 133, column: 9, scope: !103)
!122 = !DILocation(line: 0, scope: !103)
!123 = !DILocation(line: 0, scope: !105)
!124 = !DILocation(line: 137, column: 17, scope: !109)
!125 = !DILocation(line: 0, scope: !109)
!126 = !DILocation(line: 135, column: 41, scope: !111)
!127 = !DILocation(line: 135, column: 34, scope: !111)
!128 = !DILocation(line: 135, column: 13, scope: !105)
!129 = distinct !{!129, !128, !130, !51}
!130 = !DILocation(line: 150, column: 13, scope: !105)
!131 = !DILocation(line: 133, column: 30, scope: !107)
!132 = distinct !{!132, !121, !133, !51}
!133 = !DILocation(line: 151, column: 9, scope: !103)
!134 = !DILocation(line: 139, column: 21, scope: !113)
!135 = !DILocation(line: 144, column: 25, scope: !114)
!136 = !DILocation(line: 0, scope: !113)
!137 = !DILocation(line: 141, column: 33, scope: !138)
!138 = distinct !DILexicalBlock(scope: !139, file: !1, line: 141, column: 29)
!139 = distinct !DILexicalBlock(scope: !140, file: !1, line: 139, column: 71)
!140 = distinct !DILexicalBlock(scope: !113, file: !1, line: 139, column: 21)
!141 = !DILocation(line: 141, column: 29, scope: !139)
!142 = !DILocation(line: 142, column: 43, scope: !138)
!143 = !DILocation(line: 142, column: 29, scope: !138)
!144 = !DILocation(line: 142, column: 41, scope: !138)
!145 = !DILocation(line: 139, column: 67, scope: !140)
!146 = !DILocation(line: 139, column: 46, scope: !140)
!147 = distinct !{!147, !134, !148, !51}
!148 = !DILocation(line: 143, column: 21, scope: !113)
!149 = !DILocation(line: 137, column: 63, scope: !115)
!150 = !DILocation(line: 137, column: 42, scope: !115)
!151 = distinct !{!151, !124, !152, !51}
!152 = !DILocation(line: 149, column: 17, scope: !109)
!153 = !DILocation(line: 145, column: 39, scope: !154)
!154 = distinct !DILexicalBlock(scope: !114, file: !1, line: 144, column: 25)
!155 = !DILocation(line: 145, column: 25, scope: !154)
!156 = !DILocation(line: 145, column: 37, scope: !154)
!157 = !DILocation(line: 169, column: 16, scope: !158)
!158 = distinct !DILexicalBlock(scope: !101, file: !1, line: 169, column: 14)
!159 = !DILocation(line: 169, column: 14, scope: !101)
!160 = !DILocalVariable(name: "M", arg: 1, scope: !161, file: !1, line: 83, type: !12)
!161 = distinct !DISubprogram(name: "trans_basic", scope: !1, file: !1, line: 83, type: !10, scopeLine: 84, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !162)
!162 = !{!160, !163, !164, !165, !166, !167, !169}
!163 = !DILocalVariable(name: "N", arg: 2, scope: !161, file: !1, line: 83, type: !12)
!164 = !DILocalVariable(name: "A", arg: 3, scope: !161, file: !1, line: 83, type: !15)
!165 = !DILocalVariable(name: "B", arg: 4, scope: !161, file: !1, line: 83, type: !15)
!166 = !DILocalVariable(name: "tmp", arg: 5, scope: !161, file: !1, line: 84, type: !20)
!167 = !DILocalVariable(name: "i", scope: !168, file: !1, line: 88, type: !12)
!168 = distinct !DILexicalBlock(scope: !161, file: !1, line: 88, column: 5)
!169 = !DILocalVariable(name: "j", scope: !170, file: !1, line: 89, type: !12)
!170 = distinct !DILexicalBlock(scope: !171, file: !1, line: 89, column: 9)
!171 = distinct !DILexicalBlock(scope: !172, file: !1, line: 88, column: 36)
!172 = distinct !DILexicalBlock(scope: !168, file: !1, line: 88, column: 5)
!173 = !DILocation(line: 0, scope: !161, inlinedAt: !174)
!174 = distinct !DILocation(line: 170, column: 9, scope: !158)
!175 = !DILocation(line: 0, scope: !168, inlinedAt: !174)
!176 = !DILocation(line: 88, column: 26, scope: !172, inlinedAt: !174)
!177 = !DILocation(line: 88, column: 5, scope: !168, inlinedAt: !174)
!178 = !DILocation(line: 0, scope: !170, inlinedAt: !174)
!179 = !DILocation(line: 89, column: 9, scope: !170, inlinedAt: !174)
!180 = !DILocation(line: 90, column: 23, scope: !181, inlinedAt: !174)
!181 = distinct !DILexicalBlock(scope: !182, file: !1, line: 89, column: 40)
!182 = distinct !DILexicalBlock(scope: !170, file: !1, line: 89, column: 9)
!183 = !DILocation(line: 90, column: 13, scope: !181, inlinedAt: !174)
!184 = !DILocation(line: 90, column: 21, scope: !181, inlinedAt: !174)
!185 = !DILocation(line: 89, column: 36, scope: !182, inlinedAt: !174)
!186 = !DILocation(line: 89, column: 30, scope: !182, inlinedAt: !174)
!187 = distinct !{!187, !179, !188, !51}
!188 = !DILocation(line: 91, column: 9, scope: !170, inlinedAt: !174)
!189 = !DILocation(line: 88, column: 32, scope: !172, inlinedAt: !174)
!190 = distinct !{!190, !177, !191, !51}
!191 = !DILocation(line: 92, column: 5, scope: !168, inlinedAt: !174)
!192 = !DILocalVariable(name: "M", arg: 1, scope: !193, file: !1, line: 103, type: !12)
!193 = distinct !DISubprogram(name: "trans_tmp", scope: !1, file: !1, line: 103, type: !10, scopeLine: 104, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !194)
!194 = !{!192, !195, !196, !197, !198, !199, !201, !205, !208}
!195 = !DILocalVariable(name: "N", arg: 2, scope: !193, file: !1, line: 103, type: !12)
!196 = !DILocalVariable(name: "A", arg: 3, scope: !193, file: !1, line: 103, type: !15)
!197 = !DILocalVariable(name: "B", arg: 4, scope: !193, file: !1, line: 103, type: !15)
!198 = !DILocalVariable(name: "tmp", arg: 5, scope: !193, file: !1, line: 104, type: !20)
!199 = !DILocalVariable(name: "i", scope: !200, file: !1, line: 108, type: !12)
!200 = distinct !DILexicalBlock(scope: !193, file: !1, line: 108, column: 5)
!201 = !DILocalVariable(name: "j", scope: !202, file: !1, line: 109, type: !12)
!202 = distinct !DILexicalBlock(scope: !203, file: !1, line: 109, column: 9)
!203 = distinct !DILexicalBlock(scope: !204, file: !1, line: 108, column: 36)
!204 = distinct !DILexicalBlock(scope: !200, file: !1, line: 108, column: 5)
!205 = !DILocalVariable(name: "di", scope: !206, file: !1, line: 110, type: !12)
!206 = distinct !DILexicalBlock(scope: !207, file: !1, line: 109, column: 40)
!207 = distinct !DILexicalBlock(scope: !202, file: !1, line: 109, column: 9)
!208 = !DILocalVariable(name: "dj", scope: !206, file: !1, line: 111, type: !12)
!209 = !DILocation(line: 0, scope: !193, inlinedAt: !210)
!210 = distinct !DILocation(line: 172, column: 9, scope: !158)
!211 = !DILocation(line: 0, scope: !200, inlinedAt: !210)
!212 = !DILocation(line: 108, column: 26, scope: !204, inlinedAt: !210)
!213 = !DILocation(line: 108, column: 5, scope: !200, inlinedAt: !210)
!214 = !DILocation(line: 0, scope: !202, inlinedAt: !210)
!215 = !DILocation(line: 109, column: 9, scope: !202, inlinedAt: !210)
!216 = !DILocation(line: 0, scope: !206, inlinedAt: !210)
!217 = !DILocation(line: 111, column: 27, scope: !206, inlinedAt: !210)
!218 = !DILocation(line: 112, column: 32, scope: !206, inlinedAt: !210)
!219 = !DILocation(line: 112, column: 24, scope: !206, inlinedAt: !210)
!220 = !DILocation(line: 112, column: 13, scope: !206, inlinedAt: !210)
!221 = !DILocation(line: 112, column: 30, scope: !206, inlinedAt: !210)
!222 = !DILocation(line: 113, column: 13, scope: !206, inlinedAt: !210)
!223 = !DILocation(line: 113, column: 21, scope: !206, inlinedAt: !210)
!224 = !DILocation(line: 109, column: 36, scope: !207, inlinedAt: !210)
!225 = !DILocation(line: 109, column: 30, scope: !207, inlinedAt: !210)
!226 = distinct !{!226, !215, !227, !51}
!227 = !DILocation(line: 114, column: 9, scope: !202, inlinedAt: !210)
!228 = !DILocation(line: 108, column: 32, scope: !204, inlinedAt: !210)
!229 = distinct !{!229, !213, !230, !51}
!230 = !DILocation(line: 115, column: 5, scope: !200, inlinedAt: !210)
!231 = !DILocation(line: 173, column: 1, scope: !92)
!232 = !DILocation(line: 0, scope: !161)
!233 = !DILocation(line: 0, scope: !168)
!234 = !DILocation(line: 88, column: 26, scope: !172)
!235 = !DILocation(line: 88, column: 5, scope: !168)
!236 = !DILocation(line: 0, scope: !170)
!237 = !DILocation(line: 89, column: 9, scope: !170)
!238 = !DILocation(line: 89, column: 36, scope: !182)
!239 = !DILocation(line: 90, column: 23, scope: !181)
!240 = !{!241}
!241 = distinct !{!241, !242}
!242 = distinct !{!242, !"LVerDomain"}
!243 = !DILocation(line: 90, column: 21, scope: !181)
!244 = !{!245}
!245 = distinct !{!245, !242}
!246 = distinct !{!246, !237, !247, !51, !248}
!247 = !DILocation(line: 91, column: 9, scope: !170)
!248 = !{!"llvm.loop.isvectorized", i32 1}
!249 = !DILocation(line: 90, column: 13, scope: !181)
!250 = !DILocation(line: 89, column: 30, scope: !182)
!251 = distinct !{!251, !237, !247, !51, !248}
!252 = !DILocation(line: 88, column: 32, scope: !172)
!253 = distinct !{!253, !235, !254, !51}
!254 = !DILocation(line: 92, column: 5, scope: !168)
!255 = !DILocation(line: 95, column: 1, scope: !161)
!256 = !DILocation(line: 0, scope: !193)
!257 = !DILocation(line: 0, scope: !200)
!258 = !DILocation(line: 108, column: 26, scope: !204)
!259 = !DILocation(line: 108, column: 5, scope: !200)
!260 = !DILocation(line: 0, scope: !202)
!261 = !DILocation(line: 109, column: 9, scope: !202)
!262 = !DILocation(line: 0, scope: !206)
!263 = !DILocation(line: 111, column: 27, scope: !206)
!264 = !DILocation(line: 112, column: 32, scope: !206)
!265 = !DILocation(line: 112, column: 24, scope: !206)
!266 = !DILocation(line: 112, column: 13, scope: !206)
!267 = !DILocation(line: 112, column: 30, scope: !206)
!268 = !DILocation(line: 113, column: 13, scope: !206)
!269 = !DILocation(line: 113, column: 21, scope: !206)
!270 = !DILocation(line: 109, column: 36, scope: !207)
!271 = !DILocation(line: 109, column: 30, scope: !207)
!272 = distinct !{!272, !261, !273, !51}
!273 = !DILocation(line: 114, column: 9, scope: !202)
!274 = !DILocation(line: 108, column: 32, scope: !204)
!275 = distinct !{!275, !259, !276, !51}
!276 = !DILocation(line: 115, column: 5, scope: !200)
!277 = !DILocation(line: 118, column: 1, scope: !193)
