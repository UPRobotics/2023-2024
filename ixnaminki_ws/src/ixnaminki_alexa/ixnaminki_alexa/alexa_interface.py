from flask import Flask
from ask_sdk_core.skill_builder import SkillBuilder
from flask_ask_sdk.skill_adapter import SkillAdapter
from ask_sdk_core.dispatch_components import AbstractRequestHandler
from ask_sdk_core.utils import is_request_type, is_intent_name
from ask_sdk_core.handler_input import HandlerInput
from ask_sdk_model import Response
from ask_sdk_model.ui import SimpleCard
from ask_sdk_core.dispatch_components import AbstractExceptionHandler



app = Flask(__name__)
class LaunchRequestHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_request_type("LaunchRequest")(handler_input)

    def handle(self, handler_input):
        speech_text = "Ixnaminki olinki está a la orden"

        handler_input.response_builder.speak(speech_text).set_card(
            SimpleCard("Hello World", speech_text)).set_should_end_session(
            False)
        return handler_input.response_builder.response

class MoveFlipperUpIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        return is_intent_name("LevantarFlippersDelanteros")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speech_text = "Levantando flippers delanteros"

        handler_input.response_builder.speak(speech_text).ask(speech_text).set_card(
            SimpleCard("Flippers delanteros arriba", speech_text))
        return handler_input.response_builder.response

class AllExceptionHandler(AbstractExceptionHandler):

    def can_handle(self, handler_input, exception):
        return True

    def handle(self, handler_input, exception):
        print(exception)

        speech = "Disculpe, ¿puede repetir la instrucción?"
        handler_input.response_builder.speak(speech).ask(speech)
        return handler_input.response_builder.response

skill_builder = SkillBuilder()
skill_builder.add_request_handler(LaunchRequestHandler())
skill_builder.add_request_handler(MoveFlipperUpIntentHandler())
skill_builder.add_exception_handler(AllExceptionHandler())

# Register your intent handlers to the skill_builder object

skill_adapter = SkillAdapter(
    skill=skill_builder.create(), skill_id="amzn1.ask.skill.8a7b2446-2a4e-4781-9a3d-b05e7cf19b01", app=app)

@app.route("/")
def invoke_skill():
    return skill_adapter.dispatch_request()

skill_adapter.register(app=app, route='/')

if __name__ == "__main__":
    app.run()