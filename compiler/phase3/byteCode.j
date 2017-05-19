.source input.txt
.class public test
.super java/lang/Object
.method public <init>()V
aload_0
invokenonvirtual java/lang/Object/<init>()V
return
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 100
.limit stack 100
0:	iconst_1
1:	iconst_3
2:	imul
3:	istore_1
4:	iload_1
5:	iconst_5
6:	if_icmpge	18
9:	bipush	6
11:	bipush	10
13:	imul
14:	istore_1
15:	goto	20
18:	iconst_5
19:	istore_1
20:	getstatic      java/lang/System/out Ljava/io/PrintStream;
23:	iload_1
24:	invokevirtual java/io/PrintStream/println(I)V
27: return
return
.end method
