#!/usr/bin/env python3
from flask import Flask
from ask_sdk_core.skill_builder import SkillBuilder
from flask_ask_sdk.skill_adapter import SkillAdapter
from ask_sdk_core.utils import is_request_type, is_intent_name
from ask_sdk_core.handler_input import HandlerInput
from ask_sdk_model import Response
from ask_sdk_model.ui import SimpleCard
from ask_sdk_core.dispatch_components import AbstractRequestHandler, AbstractExceptionHandler
import rclpy
from rclpy.node import Node
import threading
from rclpy.action import ActionClient
from posicion.srv import Target

threading.Thread(target=lambda: rclpy.init()).start()
action_client = ActionClient(Node('alexa_interface'), Target, "plan_server")

app = Flask(__name__)

x = 0
y = 0
z = 300

class LaunchRequestHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return is_request_type("LaunchRequest")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speech_text = "Ixnaminki Olinki esta a la orden"

        handler_input.response_builder.speak(speech_text).set_card(
            SimpleCard("Online", speech_text)).set_should_end_session(
            False)

        goal = Target.Goal()
        goal.x = 0
        goal.y = 0
        goal.z = 300
        action_client.send_goal_async(goal)

        return handler_input.response_builder.response


class LevantarIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return is_intent_name("levantar")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speech_text = "levantando brazo"

        handler_input.response_builder.speak(speech_text).set_card(
            SimpleCard("moveup", speech_text)).set_should_end_session(
            True)

        goal = Target.Goal()
        goal.x = 0
        goal.y = 0
        goal.z = 700
        action_client.send_goal_async(goal)

        return handler_input.response_builder.response


class SleepIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return is_intent_name("Dormir")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speech_text = "Ixnaminki Olinki se despide"

        handler_input.response_builder.speak(speech_text).set_card(
            SimpleCard("Sleep", speech_text)).set_should_end_session(
            True)

        goal = Target.Goal()
        goal.x = 0
        goal.y = 0
        goal.z = 300
        action_client.send_goal_async(goal)

        return handler_input.response_builder.response

class MoverZIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return is_intent_name("MoverZ")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speech_text = "Moviendo hacia arriba"

        handler_input.response_builder.speak(speech_text).set_card(
            SimpleCard("movez", speech_text)).set_should_end_session(
            True)

        z=z+50        
        goal = Target.Goal()
        goal.x = x
        goal.y = y
        goal.z = z
        action_client.send_goal_async(goal)

        return handler_input.response_builder.response
    
class MoverXIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return is_intent_name("MoverX")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speech_text = "Moviendo en eje equis positivo"

        handler_input.response_builder.speak(speech_text).set_card(
            SimpleCard("movex", speech_text)).set_should_end_session(
            True)

        x=x+50        
        goal = Target.Goal()
        goal.x = x
        goal.y = y
        goal.z = z
        action_client.send_goal_async(goal)

        return handler_input.response_builder.response
    
class MoverYIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return is_intent_name("MoverY")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speech_text = "Moviendo en eje ye positivo"

        handler_input.response_builder.speak(speech_text).set_card(
            SimpleCard("movey", speech_text)).set_should_end_session(
            True)

        y=y+50        
        goal = Target.Goal()
        goal.x = x
        goal.y = y
        goal.z = z
        action_client.send_goal_async(goal)

        return handler_input.response_builder.response

class RetrocederZIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return is_intent_name("RetrocederZ")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speech_text = "Retrocediendo hacia arriba"

        handler_input.response_builder.speak(speech_text).set_card(
            SimpleCard("retrocedez", speech_text)).set_should_end_session(
            True)

        z=z-50        
        goal = Target.Goal()
        goal.x = x
        goal.y = y
        goal.z = z
        action_client.send_goal_async(goal)

        return handler_input.response_builder.response
    
class RetrocederXIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return is_intent_name("RetrocederX")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speech_text = "Retrocediendo en eje equis positivo"

        handler_input.response_builder.speak(speech_text).set_card(
            SimpleCard("retrocedex", speech_text)).set_should_end_session(
            True)

        x=x-50        
        goal = Target.Goal()
        goal.x = x
        goal.y = y
        goal.z = z
        action_client.send_goal_async(goal)

        return handler_input.response_builder.response
    
class RetrocederYIntentHandler(AbstractRequestHandler):
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return is_intent_name("RetrocederY")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speech_text = "Retrocediendo en eje ye positivo"

        handler_input.response_builder.speak(speech_text).set_card(
            SimpleCard("retrocedey", speech_text)).set_should_end_session(
            True)

        y=y-50        
        goal = Target.Goal()
        goal.x = x
        goal.y = y
        goal.z = z
        action_client.send_goal_async(goal)

        return handler_input.response_builder.response



class AllExceptionHandler(AbstractExceptionHandler):

    def can_handle(self, handler_input, exception):
        # type: (HandlerInput, Exception) -> bool
        return True

    def handle(self, handler_input, exception):
        # type: (HandlerInput, Exception) -> Response

        speech = "Ixnaminki Olinki no ha entendido"
        handler_input.response_builder.speak(speech).ask(speech)
        return handler_input.response_builder.response


skill_builder = SkillBuilder()
skill_builder.add_request_handler(LaunchRequestHandler())
skill_builder.add_request_handler(MoverXIntentHandler())
skill_builder.add_request_handler(MoverYIntentHandler())
skill_builder.add_request_handler(MoverZIntentHandler())
skill_builder.add_request_handler(RetrocederXIntentHandler())
skill_builder.add_request_handler(RetrocederYIntentHandler())
skill_builder.add_request_handler(RetrocederZIntentHandler())
skill_builder.add_request_handler(SleepIntentHandler())
skill_builder.add_exception_handler(AllExceptionHandler())


skill_adapter = SkillAdapter(
    skill=skill_builder.create(), 
    skill_id="SKILL-ID",
    app=app)


skill_adapter.register(app=app, route="/")


if __name__ == '__main__':
    app.run()