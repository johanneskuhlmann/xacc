
/***********************************************************************************
 * Copyright (c) 2016, UT-Battelle
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the xacc nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Contributors:
 *   Initial API and implementation - Alex McCaskey
 *
 **********************************************************************************/
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ScaffoldCompilerTester

#include <boost/test/included/unit_test.hpp>
#include "ImprovedScaffold.hpp"

using namespace xacc::quantum;

struct F {
	F() :
			compiler(xacc::CompilerRegistry::instance()->create("improvedscaffold")) {
		BOOST_TEST_MESSAGE("setup fixture");
		BOOST_VERIFY(compiler);
	}
	~F() {
		BOOST_TEST_MESSAGE("teardown fixture");
	}

	std::shared_ptr<xacc::Compiler> compiler;
};

//____________________________________________________________________________//

BOOST_FIXTURE_TEST_SUITE( s, F )

BOOST_AUTO_TEST_CASE(checkSimpleCompile) {

	const std::string src("module eprCreation (qbit qreg[2]) {\n"
			"   H(qreg[0]);\n"
			"   CNOT(qreg[0],qreg[1]);\n"
			"}\n");

	scaffold::ImprovedScaffCCAPI api(src);

	auto fNames = api.getFunctionNames();
	for (auto f : fNames) {
		std::cout <<"F: " << f << "\n";
	}

	BOOST_VERIFY(fNames.size() ==1);
	BOOST_VERIFY(fNames[0] == "eprCreation");
}

BOOST_AUTO_TEST_CASE(checkFindIfStmts) {
	const std::string src("module teleport (qbit qreg[3]) {\n"
		"   cbit creg[2];\n"
		"   // Init qubit 0 to 1\n"
		"   X(qreg[0]);\n"
		"   // Now teleport...\n"
		"   H(qreg[1]);\n"
		"   CNOT(qreg[1],qreg[2]);\n"
		"   CNOT(qreg[0],qreg[1]);\n"
		"   H(qreg[0]);\n"
		"   creg[0] = MeasZ(qreg[0]);\n"
		"   creg[1] = MeasZ(qreg[1]);\n"
		"   if (creg[0] == 1) Z(qreg[2]);\n"
		"   if (creg[1] == 1) X(qreg[2]);\n"
		"}\n");

	scaffold::ImprovedScaffCCAPI api(src);

	api.checkIfStmts();

}
BOOST_AUTO_TEST_SUITE_END()