#!/bin/tcsh -f
#
#-------------------------------------------------------------------------
# checkdirs.sh
#-------------------------------------------------------------------------
# April 2013
# Tim Edwards, Open Circuit Design
#-------------------------------------------------------------------------
#
# This script searches for technology directories and project directories.
# It sets variables associated with each directory for other scripts to
# use.
#
# The directory hierarchy is expected to be set up as follows:
#
# <project> ----> source/
#	    ----> synthesis/
#	    ----> layout/
#	    ----> tech/<techname>	(optional)
#
# "tech" is optional if the technology is in the qflow install location,
# QFLOW_TECH_DIR/<techname>
#
# Optionally, techfiles may be in subdirectory <techname> without the
# intervening "tech" directory.  i.e.,
#
# <project> ----> <techname>
#
# Optionally, "tech" may point directly to the directory containing techfile
# data, i.e.,
#
# <project> ----> tech/
#
# If <project> is not specified on the command line, then it is assumed
# to be the current working directory.  If any standard subdirectories
# "source", "synthesis", or "layout" are not found, they will be set to
# the project directory.  The techfile directory must be found, or
# synthesis cannot proceed.
#
# Source this file using "source" to add variables to the existing shell
# environment.
# 
#-------------------------------------------------------------------------

# Environment variables override everything else:
#   QFLOW_TECH_DIR	path to technology directory
#   QFLOW_TECH		name of technology
#   QFLOW_PROJECT_ROOT	path to project top level directory
#
# The second two are checked before calling this routine so
# that there are known values for the two arguments passed
# to it.

if ($#argv != 1 && $#argv != 2) then
   echo Usage:  checkdirs.sh <technology_name> <project_name>
   exit 1
endif

set techname=${argv[1]}
if ($#argv == 2) then
   set projectpath=${argv[2]}
else
   set projectpath=`pwd`
endif

#----------------------------------------------------
# Check for standard working directories
#----------------------------------------------------

if ( -d ${projectpath}/source ) then
   set sourcedir=${projectpath}/source
else
   set sourcedir=${projectpath}
endif

if ( -d ${projectpath}/synthesis ) then
   set synthdir=${projectpath}/synthesis
else
   set synthdir=${projectpath}
endif

if ( -d ${projectpath}/layout ) then
   set layoutdir=${projectpath}/layout
else
   set layoutdir=${projectpath}
endif

#----------------------------------------------------
# Set variables from install locations here, so we
# don't have to do it in every script separately.
# Track path to scripts and binaries used by qflow
#----------------------------------------------------

set scriptdir=/usr/share/qflow/scripts
set bindir=/usr/bin

#----------------------------------------------------
# Check for the techfile (see comments at top)
#----------------------------------------------------

set techdir=`printenv QFLOW_TECH_DIR`
if ($techdir != "") then
   if ( !(-d ${techdir}/${techname}.sh )) then
      if ( -d ${techdir}/${techname} ) then
	 set techdir=${techdir}/${techname}
      else
	 set techdir=""
      endif
   endif
endif
      
if ($techdir != "") then
   exit 0
endif

if ( -d ${projectpath}/tech/${techname} ) then
   set techdir=${projectpath}/tech/${techname}
else
   if ( -d ${projectpath}/${techname} ) then
      set techdir=${projectpath}/${techname}
   else
      if ( -d ${projectpath}/tech && -r ${projectpath}/tech/${techname}.sh ) then
	 set techdir=${projectpath}/tech
      else
	 if ( -d /usr/share/qflow/tech/${techname} ) then
	    set techdir=/usr/share/qflow/tech/${techname}
	 else
            echo "Don't know about technology '${techname}'"
	    echo "Check /usr/share/qflow/tech/ for known technologies"
	 endif
      endif
   endif
endif

exit 0
#----------------------------------------------------
