# $NetBSD: Makefile,v 1.22 2023/12/27 13:19:11 wiz Exp $

V= 1.5.3c
N= SeriousSamAlphaRemake

DISTNAME=	${N}-${V}
PKGNAME=	serioussam-alpha-${V}
PKGREVISION=	1
CATEGORIES=	games
MASTER_SITES=	${MASTER_SITE_GITHUB:=tx00100xt/}
GITHUB_PROJECT=	${N}
GITHUB_TAG=	${V}
EXTRACT_SUFX=	.tar.gz

MAINTAINER=	<YOUR MAIL>
HOMEPAGE=	https://github.com/tx00100xt/SeriousSamAlphaRemake
COMMENT=	Open source modification for game Serious Sam Classic
LICENSE=	gnu-gpl-v2

WRKSRC=		${WRKDIR}/${DISTNAME}/Sources

USE_CMAKE=	yes
USE_TOOLS+=	bison flex
USE_LANGUAGES=	c gnu++11

USE_CXX_FEATURES+=	c++11

CMAKE_ARGS+=	-DUSE_ASM=OFF
CMAKE_ARGS+=	-DUSE_SYSTEM_INSTALL=ON

.include "../../mk/bsd.prefs.mk"

.if ${MACHINE_ARCH} == "armv7" || ${MACHINE_ARCH} == "aarch64"
CMAKE_ARGS+=	-DRPI4=ON
.endif

INSTALLATION_DIRS+=	man
INSTALLATION_DIRS+=	man/man1

post-install:
	${INSTALL_MAN} ${WRKDIR}/${DISTNAME}/man/serioussam-alpha.1 \
	    ${DESTDIR}${PREFIX}/man/man1

.include "../../devel/SDL2/buildlink3.mk"
.include "../../devel/zlib/buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
