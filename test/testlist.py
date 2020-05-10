# 20 january 2020
# note: python 3

import abc
import fileinput
import re
import sys

def errf(fmt, *args):
	print(fmt.format(*args), file = sys.stderr)

class command(metaclass = abc.ABCMeta):
	@classmethod
	@abc.abstractmethod
	def name(cls):
		raise NotImplementedError

	@classmethod
	@abc.abstractmethod
	def usageString(cls):
		raise NotImplementedError

	# returns the list of filenames to iterate through
	# returns None if there was an error parsing arguments
	@abc.abstractmethod
	def processArgs(self, args):
		raise NotImplementedError

	@abc.abstractmethod
	def run(self, casenames):
		raise NotImplementedError

class listCommand:
	@classmethod
	def name(cls):
		return 'list'

	@classmethod
	def usageString(cls):
		return 'list [source-files...]'

	def processArgs(self, args):
		return args

	def run(self, casenames):
		for x in casenames:
			print('Test' + x)
command.register(listCommand)

headerHeader = '''// Generated by testlist.py; do not edit
struct testingprivCase {
	const char *name;
	void (*f)(void);
};
extern const struct testingprivCase testingprivCases[];
extern const size_t testingprivNumCases;'''
headerEntryFmt = 'extern void testingprivScaffoldName(Test{})(void);'

class headerCommand:
	filename = None

	@classmethod
	def name(cls):
		return 'header'

	@classmethod
	def usageString(cls):
		return 'header header-file [source-files...]'

	def processArgs(self, args):
		if len(args) < 1:
			errf('error: output filename missing')
			return None
		self.filename = args[0]
		return args[1:]

	def run(self, casenames):
		with open(self.filename, 'wt') as f:
			print(headerHeader, file = f)
			for x in casenames:
				print(headerEntryFmt.format(x), file = f)
command.register(headerCommand)

sourceHeader = '''// Generated by testlist.py; do not edit
#include "test.h"
const struct testingprivCase testingprivCases[] = {'''
sourceEntryFmt = '	{{ "Test{0}", testingprivScaffoldName(Test{0}) }},'
sourceFooter = '''}};
const size_t testingprivNumCases = {};'''

class sourceCommand:
	filename = None

	@classmethod
	def name(cls):
		return 'source'

	@classmethod
	def usageString(cls):
		return 'source source-file [source-files...]'

	def processArgs(self, args):
		if len(args) < 1:
			errf('error: output filename missing')
			return None
		self.filename = args[0]
		return args[1:]

	def run(self, casenames):
		with open(self.filename, 'wt') as f:
			print(sourceHeader, file = f)
			for x in casenames:
				print(sourceEntryFmt.format(x), file = f)
			print(sourceFooter.format(len(casenames)), file = f)
command.register(sourceCommand)

commands = [
	listCommand,
	headerCommand,
	sourceCommand,
]

def usage():
	errf('usage:')
	errf('{} help', sys.argv[0])
	for cmd in commands:
		errf('{} {}', sys.argv[0], cmd.usageString())
	sys.exit(1)

def main():
	rTest = re.compile('^(?:Test|TestNoInit)\(([A-Za-z0-9_]+)\)$')
	rAllCalls = re.compile('^(?:allcallsCase)\(([A-Za-z0-9_]+),')

	if len(sys.argv) < 2:
		usage()
	cmdname = sys.argv[1]
	if cmdname == 'help':
		usage()
	cmdtype = None
	for cmd in commands:
		if cmd.name() == cmdname:
			cmdtype = cmd
			break
	if cmdtype is None:
		errf('error: unknown command {!r}', cmdname)
		usage()

	cmd = cmdtype()
	files = cmd.processArgs(sys.argv[2:])
	if files is None:
		usage()

	casenames = []
	for line in fileinput.input(files = files):
		match = rTest.match(line)
		if match is not None:
			casenames.append(match.group(1))
		match = rAllCalls.match(line)
		if match is not None:
			f = match.group(1)
			# noinitwrongthread.c
			casenames.append('CallBeforeInitIsProgrammerError_' + f)
			casenames.append('CallOnWrongThreadIsProgrammerError_' + f)
	cmd.run(sorted(casenames))

main()
