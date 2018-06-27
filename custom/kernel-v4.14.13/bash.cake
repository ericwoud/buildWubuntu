#!/bin/bash
$sudo git clone --depth 1 https://github.com/dtaht/sch_cake.git .tmp
$sudo mv -f .tmp/*.c .tmp/*.h $kerneldir/net/sched/
$sudo rm -fr .tmp
$sudo patch -d $kerneldir -p1 -N -r - <<'EOF'
diff -Naur a/net/sched/Kconfig b/net/sched/Kconfig
--- a/net/sched/Kconfig	2018-01-26 19:06:46.000000000 +0100
+++ b/net/sched/Kconfig	2018-01-11 21:39:58.000000000 +0100
@@ -309,6 +309,11 @@
 
 	  If unsure, say N.
 
+config NET_SCH_CAKE
+	tristate "Cake (Common Applications Kept Enhanced)"
+	---help---
+	  Cake is the rollup of 3 years of deployment experience of the htb + fq_codel.
+
 config NET_SCH_INGRESS
 	tristate "Ingress/classifier-action Qdisc"
 	depends on NET_CLS_ACT
diff -Naur a/net/sched/Makefile b/net/sched/Makefile
--- a/net/sched/Makefile	2018-01-26 19:06:46.000000000 +0100
+++ b/net/sched/Makefile	2018-01-11 21:39:58.000000000 +0100
@@ -53,6 +53,7 @@
 obj-$(CONFIG_NET_SCH_FQ)	+= sch_fq.o
 obj-$(CONFIG_NET_SCH_HHF)	+= sch_hhf.o
 obj-$(CONFIG_NET_SCH_PIE)	+= sch_pie.o
+obj-$(CONFIG_NET_SCH_CAKE)      += sch_cake.o
 
 obj-$(CONFIG_NET_CLS_U32)	+= cls_u32.o
 obj-$(CONFIG_NET_CLS_ROUTE4)	+= cls_route.o
EOF
