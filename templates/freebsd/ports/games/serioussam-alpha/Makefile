PORTNAME=	serioussam-alpha
DISTNAME=	SeriousSamAlphaRemake
DISTVERSION=	1.5.3c
PORTREVISION=	1
CATEGORIES=	games

MAINTAINER= 	t.x00100x.t@yandex.ru
COMMENT=	Open source modification for game Serious Sam Classic 
WWW=		https://github.com/tx00100xt/SeriousSamAlphaRemake

LICENSE= 	GPLv2
LICENSE_FILE= 	${WRKSRC}/../LICENSE

BUILD_DEPENDS=	flex:textproc/flex

RUN_DEPENDS=	${LOCALBASE}/bin/serioussam:games/serioussam

USES=		bison:build cmake compiler:c++11-lang
USE_CXXSTD=	c++14

GH_ACCOUNT=	tx00100xt
GH_PROJECT=	SeriousSamAlphaRemake
USE_GITHUB=	yes

CMAKE_ARGS=	-DRPI4:BOOL=FALSE -DUSE_ASM:BOOL=FALSE \
	-DUSE_SYSTEM_INSTALL:BOOL=TRUE

WRKSRC_SUBDIR=	Sources

post-install:
	${INSTALL_MAN} ${WRKSRC}/../man/serioussam-alpha.1 \
		${STAGEDIR}${PREFIX}/man/man1/serioussam-alpha.1

.include <bsd.port.options.mk>

.if ${ARCH} == aarch64 || ${ARCH} == armv7
CMAKE_ON=	RPI4
.endif

.include	<bsd.port.mk>
